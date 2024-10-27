#pragma once
#include <compile/types.h>
#include <bind/DataType.h>
#include <utils/Array.h>

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
    class TypeSpecializer;
    class TemplateFunction;
    class ScriptTemplateFunction;

    class TemplateType : public bind::DataType {
        public:
            struct SpecializationData {
                bind::DataType* specializedType;
                Context* compilationContext;
                Array<TypeArgument> typeArguments;
            };

            TemplateType(const String& name, u32 typeArgumentCount, bind::Namespace* ns = nullptr);
            virtual ~TemplateType();

            u32 getTypeArgumentCount() const;
            void setPostProcessMask(u32 mask);

            bind::DataType* specialize(Context* ctx, const Array<bind::DataType*>& typeArgs);
            bind::DataType* specialize(Context* ctx, CompileHandler* ch, const Array<parse::Node*>& typeArgs);
            virtual bool onSpecialization(TypeSpecializer* specializer) = 0;

        protected:
            friend class Context;

            void setMomentarySpecializationData(SpecializationData* data);
            void setMomentarySpecializer(TypeSpecializer* specializer);
            void cleanupMomentarySpecializationInfo();

            bool doSpecialization();

        private:
            u32 m_typeArgumentCount;
            u32 m_postProcessMask;

            // the following information is contextually related to individual
            // occurrences of template specialization. When a template type is
            // being specialized all of this data is reset to information that
            // is specifically related to /that/ specialization.

            SpecializationData* m_specializationData;
            TypeSpecializer* m_specializer;
    };
};