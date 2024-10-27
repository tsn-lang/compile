#include <compile/TemplateFunction.h>
#include <compile/TypeArgument.h>
#include <compile/Context.h>
#include <compile/ast/CompileHandler.h>
#include <compile/CompiledFunction.h>
#include <compile/FunctionSpecializer.h>
#include <compile/Compiler.h>
#include <codegen/interfaces/IBackend.h>
#include <parse/misc/Node.h>
#include <bind/Function.h>
#include <bind/Registry.h>
#include <utils/Array.hpp>

namespace compile {
    TemplateFunction::TemplateFunction(
        const String& name,
        u32 argumentCount,
        u32 typeArgumentCount,
        bind::Namespace* ns
    ) : m_argumentCount(argumentCount), m_typeArgumentCount(typeArgumentCount), m_postProcessMask(0xFFFFFFFF),
        m_name(name), m_namespace(ns ? ns : bind::Registry::GlobalNamespace()), m_specializationData(nullptr),
        m_specializer(nullptr), m_generatedSignature(nullptr)
    {
    }

    TemplateFunction::~TemplateFunction() {
    }

    u32 TemplateFunction::getArgumentCount() const {
        return m_argumentCount;
    }

    u32 TemplateFunction::getTypeArgumentCount() const {
        return m_typeArgumentCount;
    }

    const String& TemplateFunction::getName() const {
        return m_name;
    }

    bind::Namespace* TemplateFunction::getNamespace() const {
        return m_namespace;
    }

    void TemplateFunction::setPostProcessMask(u32 mask) {
        m_postProcessMask = mask;
    }

    bind::Function* TemplateFunction::specialize(Context* ctx, const Array<bind::DataType*>& typeArgs) {
        bind::Function* result = nullptr;
        bind::FunctionType* signature = nullptr;
        bool sigDidExist = false;
        
        TemplateFunction::SpecializationData sp;
        sp.compilationContext = ctx;
        sp.specializedFunction = nullptr;

        FunctionSpecializer ts(this, ctx, sp.typeArguments);
        
        for (u32 i = 0;i < typeArgs.size();i++) {
            sp.typeArguments.emplace(ctx, typeArgs[i], nullptr);
        }

        setMomentarySpecializer(&ts);
        setMomentarySpecializationData(&sp);

        bool didError = false;

        try {
            if (!onSpecialization(&ts)) {
                didError = true;
                ctx->logError("Could not specialize function '%s' with the provided arguments", m_name.c_str());
            } else {
                signature = ts.generateSignature(&sigDidExist);

                if (signature) {
                    if (!sigDidExist) {
                        setMomentaryGeneratedSignature(signature);
                    }

                    sp.specializedFunction = new bind::Function(
                        m_name,
                        signature,
                        getNamespace()
                    );

                    if (!doSpecialization()) {
                        didError = true;
                        ctx->logError("Could not specialize function '%s' with the provided arguments", m_name.c_str());
                    } else {
                        result = sp.specializedFunction;
                        ctx->onFunctionGenerated(result);
                        if (!sigDidExist) ctx->onTypeGenerated(signature);
                    }
                }
            }
        } catch (const Exception& e) {
            didError = true;
            ctx->logError(
                "Encountered exception while specializing function '%s': %s",
                m_name.c_str(),
                e.what()
            );
        }

        cleanupMomentarySpecializationInfo();

        if (didError) {
            result = nullptr;

            if (sp.specializedFunction) {
                delete sp.specializedFunction;
                sp.specializedFunction = nullptr;
            }

            if (signature && !sigDidExist) {
                bind::Registry::Remove(signature);
                delete signature;
                signature = nullptr;
            }
        }
        
        return result;
    }
    
    bind::Function* TemplateFunction::specialize(Context* ctx, CompileHandler* ch, const Array<parse::Node*>& typeArgs) {
        bind::Function* result = nullptr;
        bind::FunctionType* signature = nullptr;
        bool sigDidExist = false;
        
        TemplateFunction::SpecializationData sp;
        sp.compilationContext = ctx;
        sp.specializedFunction = nullptr;

        FunctionSpecializer ts(this, ctx, sp.typeArguments);
        
        for (u32 i = 0;i < typeArgs.size();i++) {
            typeArgs[i]->acceptVisitor(ch);
            bind::DataType* tp = ch->popLastType();
            sp.typeArguments.emplace(ctx, tp, typeArgs[i]);
        }

        setMomentarySpecializer(&ts);
        setMomentarySpecializationData(&sp);

        bool didError = false;

        try {
            if (!onSpecialization(&ts)) {
                didError = true;
                ctx->logError("Could not specialize function '%s' with the provided arguments", m_name.c_str());
            } else {
                signature = ts.generateSignature(&sigDidExist);

                if (signature) {
                    if (!sigDidExist) {
                        setMomentaryGeneratedSignature(signature);
                    }

                    sp.specializedFunction = new bind::Function(
                        m_name,
                        signature,
                        getNamespace()
                    );

                    if (!doSpecialization()) {
                        didError = true;
                        ctx->logError("Could not specialize function '%s' with the provided arguments", m_name.c_str());
                    } else {
                        result = sp.specializedFunction;
                        ctx->onFunctionGenerated(result);
                        if (!sigDidExist) ctx->onTypeGenerated(signature);
                    }
                }
            }
        } catch (const Exception& e) {
            didError = true;
            ctx->logError(
                "Encountered exception while specializing function '%s': %s",
                m_name.c_str(),
                e.what()
            );
        }

        cleanupMomentarySpecializationInfo();

        if (didError) {
            result = nullptr;

            if (sp.specializedFunction) {
                delete sp.specializedFunction;
                sp.specializedFunction = nullptr;
            }

            if (signature && !sigDidExist) {
                bind::Registry::Remove(signature);
                delete signature;
                signature = nullptr;
            }
        }
        
        return result;
    }

    void TemplateFunction::setMomentarySpecializationData(SpecializationData* data) {
        m_specializationData = data;
    }

    void TemplateFunction::setMomentarySpecializer(FunctionSpecializer* specializer) {
        m_specializer = specializer;
    }

    void TemplateFunction::setMomentaryGeneratedSignature(bind::FunctionType* sig) {
        m_generatedSignature = sig;
    }

    void TemplateFunction::cleanupMomentarySpecializationInfo() {
        m_specializationData = nullptr;
        m_specializer = nullptr;
        m_generatedSignature = nullptr;
    }

    bool TemplateFunction::doSpecialization() {
        m_specializationData->compilationContext->logDebug(
            "Specializing template function '%s': '%s'",
            getName().c_str(),
            m_specializationData->specializedFunction->getName().c_str()
        );

        codegen::FunctionBuilder fb(m_specializationData->specializedFunction);
        fb.enableValidation();

        bool didSucceed = true;

        try {
            onCodeGen(&fb);
        } catch (const Exception& e) {
            m_specializationData->compilationContext->logError(
                "User defined template function generator for '%s' failed validation with the following error",
                m_specializationData->specializedFunction->getName().c_str()
            );
            m_specializationData->compilationContext->logInfo(e.what());

            didSucceed = false;
        }

        if (didSucceed) {
            try {
                Compiler* c = m_specializationData->compilationContext->getCompiler();
                if (!c->getBackend()->process(&fb, m_postProcessMask)) {
                    m_specializationData->compilationContext->logError(
                        "Compiler backend failed to process code for user defined template function '%s'",
                        m_specializationData->specializedFunction->getName().c_str()
                    );

                    didSucceed = false;
                }
            } catch (const Exception& e) {
                m_specializationData->compilationContext->logError(
                    "Compiler backend failed to process code for user defined template function '%s' with the following error",
                    m_specializationData->specializedFunction->getName().c_str()
                );
                m_specializationData->compilationContext->logInfo(e.what());

                didSucceed = false;
            }
        }

        return didSucceed;
    }
};