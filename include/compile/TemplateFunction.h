#pragma once
#include <compile/types.h>
#include <utils/Array.h>
#include <utils/String.h>

namespace bind {
    class DataType;
    class Function;
    class Namespace;
    class FunctionType;
};

namespace parse {
    class Node;
};

namespace codegen {
    class FunctionBuilder;
};

namespace compile {
    class Context;
    class CompileHandler;
    class TypeArgument;
    class FunctionSpecializer;

    class TemplateFunction {
        public:
            struct SpecializationData {
                bind::Function* specializedFunction;
                Context* compilationContext;
                Array<TypeArgument> typeArguments;
            };

            TemplateFunction(
                const String& name,
                u32 argumentCount,
                u32 typeArgumentCount,
                bind::Namespace* ns = nullptr
            );
            virtual ~TemplateFunction();

            u32 getArgumentCount() const;
            u32 getTypeArgumentCount() const;
            const String& getName() const;
            bind::Namespace* getNamespace() const;
            void setPostProcessMask(u32 mask);

            bind::Function* specialize(Context* ctx, const Array<bind::DataType*>& typeArgs);
            bind::Function* specialize(Context* ctx, CompileHandler* ch, const Array<parse::Node*>& typeArgs);
            virtual bool onSpecialization(FunctionSpecializer* specializer) = 0;
            virtual void onCodeGen(codegen::FunctionBuilder* code) = 0;

        protected:
            friend class Context;

            void setMomentarySpecializationData(SpecializationData* data);
            void setMomentarySpecializer(FunctionSpecializer* specializer);
            void setMomentaryGeneratedSignature(bind::FunctionType* sig);
            void cleanupMomentarySpecializationInfo();

            bool doSpecialization();

        private:
            u32 m_argumentCount;
            u32 m_typeArgumentCount;
            u32 m_postProcessMask;
            String m_name;
            bind::Namespace* m_namespace;

            // the following information is contextually related to individual
            // occurrences of template specialization. When a template func is
            // being specialized all of this data is reset to information that
            // is specifically related to /that/ specialization.

            SpecializationData* m_specializationData;
            FunctionSpecializer* m_specializer;
            bind::FunctionType* m_generatedSignature;
    };
};