#include <compile/TemplateType.h>
#include <compile/Context.h>
#include <compile/CompiledFunction.h>
#include <compile/TypeSpecializer.h>
#include <compile/Compiler.h>
#include <codegen/interfaces/IBackend.h>
#include <bind/util/meta.hpp>
#include <bind/Function.h>
#include <utils/Array.hpp>
#include <utils/interfaces/IWithUserData.hpp>

namespace compile {
    TemplateType::TemplateType(const String& name, u32 typeArgumentCount, bind::Namespace*ns)
        : bind::DataType(name, bind::meta<void>(), ns), m_typeArgumentCount(typeArgumentCount),
        m_postProcessMask(0xFFFFFFFF), m_specializationData(nullptr), m_specializer(nullptr)
    {
        tsn_type_userdata& data = getUserData<tsn_type_userdata>();
        data.flags.indicator_bit = 1;
        data.flags.is_template = 1;
    }

    TemplateType::~TemplateType() {
    }

    u32 TemplateType::getTypeArgumentCount() const {
        return m_typeArgumentCount;
    }

    void TemplateType::setMomentarySpecializationData(SpecializationData* data) {
        m_specializationData = data;
    }

    void TemplateType::setMomentarySpecializer(TypeSpecializer* specializer) {
        m_specializer = specializer;
    }

    void TemplateType::addMomentaryGeneratedSignature(bind::FunctionType* sig) {
        m_generatedSignatures.push(sig);
    }
    
    const Array<TemplateType::MethodBuilder>& TemplateType::getMethods() const {
        return m_methods;
    }

    void TemplateType::cleanupMomentarySpecializationInfo() {
        m_specializationData = nullptr;
        m_specializer = nullptr;
        m_generatedSignatures.clear();
        m_methods.clear();
    }

    bool TemplateType::doSpecialization() {
        m_specializationData->compilationContext->logDebug(
            "Specializing template '%s': '%s'",
            getName().c_str(),
            m_specializationData->specializedType->getName().c_str()
        );

        for (u32 i = 0;i < m_methods.size();i++) {
            MethodBuilder& m = m_methods[i];

            m_specializationData->compilationContext->logDebug(
                "Generating user defined method '%s::%s'",
                m_specializationData->specializedType->getName().c_str(),
                m.name.c_str()
            );

            bind::Function* fn = new bind::Function(
                m.name,
                m.signature,
                m_specializationData->specializedType->getOwnNamespace()
            );

            codegen::FunctionBuilder fb(fn);
            fb.enableValidation();

            bool didSucceed = true;

            try {
                (*this.*m.methodPtr)(&fb);
            } catch (const Exception& e) {
                m_specializationData->compilationContext->logError(
                    "User defined template method generator for '%s::%s' failed validation with the following error",
                    m_specializationData->specializedType->getName().c_str(),
                    m.name.c_str()
                );
                m_specializationData->compilationContext->logInfo(e.what());

                didSucceed = false;
            }

            if (didSucceed) {
                try {
                    Compiler* c = m_specializationData->compilationContext->getCompiler();
                    if (!c->getBackend()->process(&fb, m_postProcessMask)) {
                        m_specializationData->compilationContext->logError(
                            "Compiler backend failed to process code for user defined template method '%s::%s'",
                            m_specializationData->specializedType->getName().c_str(),
                            m.name.c_str()
                        );

                        didSucceed = false;
                    }
                } catch (const Exception& e) {
                    m_specializationData->compilationContext->logError(
                        "Compiler backend failed to process code for user defined template method '%s::%s' with the following error",
                        m_specializationData->specializedType->getName().c_str(),
                        m.name.c_str()
                    );
                    m_specializationData->compilationContext->logInfo(e.what());

                    didSucceed = false;
                }
            }

            if (!didSucceed) {
                delete fn;

                for (u32 i = 0;i < m_generatedSignatures.size();i++) {
                    bind::Registry::Remove(m_generatedSignatures[i]);
                    delete m_generatedSignatures[i];
                }

                m_generatedSignatures.clear();
                return false;
            }
            
            m_specializationData->compilationContext->onFunctionGenerated(fn);
        }

        return true;
    }
};