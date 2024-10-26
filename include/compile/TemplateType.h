#pragma once
#include <compile/types.h>
#include <bind/DataType.h>
#include <utils/Array.h>

namespace parse {
    class Node;
};

namespace codegen {
    class FunctionBuilder;
    class Value;
};

namespace compile {
    class Context;
    class TypeArgument;
    class TemplateType;
    class TypeSpecializer;

    class TemplateType : public bind::DataType {
        public:
            struct SpecializationData {
                bind::DataType* specializedType;
                Context* compilationContext;
                Array<TypeArgument> typeArguments;
            };

            TemplateType(const String& name, u32 typeArgumentCount, bind::Namespace*ns = nullptr);
            ~TemplateType();

            u32 getTypeArgumentCount() const;
            void setPostProcessMask(u32 mask);

            virtual bool onSpecialization(TypeSpecializer* specializer) = 0;

            template <typename TemplateTypeCls>
            void registerMethod(
                const String& name,
                bind::FunctionType* signature,
                void (TemplateTypeCls::*methodPtr)(codegen::FunctionBuilder*)
            ) {
                m_methods.push({
                    name,
                    signature,
                    (void (TemplateType::*)(codegen::FunctionBuilder*))methodPtr
                });
            }

        protected:
            friend class Context;

            struct MethodBuilder {
                String name;
                bind::FunctionType* signature;
                void (TemplateType::*methodPtr)(codegen::FunctionBuilder*);
            };

            void setMomentarySpecializationData(SpecializationData* data);
            void setMomentarySpecializer(TypeSpecializer* specializer);
            void addMomentaryGeneratedSignature(bind::FunctionType* sig);
            const Array<MethodBuilder>& getMethods() const;
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
            Array<bind::FunctionType*> m_generatedSignatures;
            Array<MethodBuilder> m_methods;
    };
};