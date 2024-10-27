#pragma once
#include <compile/types.h>
#include <compile/TypeArgument.h>
#include <bind/DataType.h>
#include <utils/String.h>
#include <utils/Array.h>

namespace codegen {
    class FunctionBuilder;
};

namespace compile {
    class Context;
    class TypeArgument;
    class TemplateType;
    class TemplateFunction;
    class ScriptTemplateFunction;

    class TypeSpecializer {
        public:
            bind::DataType::Property& addProperty(const String& name, bind::DataType* type);
            void addMethod(TemplateFunction* method);
            void addMethod(ScriptTemplateFunction* method);
            void addStaticMethod(TemplateFunction* method);
            void addStaticMethod(ScriptTemplateFunction* method);

            const Array<TypeArgument>& getTypeArguments() const;
            bind::DataType* getSpecializedType() const;
            Context* getCompilationContext() const;

            template <typename TemplateTypeCls>
            void addConstructor(
                bind::FunctionType* signature,
                void (TemplateTypeCls::*methodPtr)(codegen::FunctionBuilder*)
            );

            template <typename TemplateTypeCls>
            void addDestructor(
                bind::FunctionType* signature,
                void (TemplateTypeCls::*methodPtr)(codegen::FunctionBuilder*)
            );

            template <typename TemplateTypeCls>
            void addMethod(
                const String& name,
                bind::FunctionType* signature,
                void (TemplateTypeCls::*methodPtr)(codegen::FunctionBuilder*)
            );

            template <typename TemplateTypeCls>
            void addStaticMethod(
                const String& name,
                bind::FunctionType* signature,
                void (TemplateTypeCls::*methodPtr)(codegen::FunctionBuilder*)
            );

        protected:
            friend class TemplateType;

            struct MethodBuilder {
                String name;
                bind::DataType::Property::Flags flags;
                bind::FunctionType* signature;
                void (TemplateType::*methodPtr)(codegen::FunctionBuilder*);
            };

            TypeSpecializer(
                TemplateType* templ,
                Context* compilationContext,
                const Array<TypeArgument>& typeArgs
            );
            virtual ~TypeSpecializer();

            bind::DataType* claimType();

            TemplateType* m_specializedType;
            bind::DataType* m_generatedType;
            Context* m_compilation;
            Array<MethodBuilder> m_methods;
            bool m_hasDtor;
            const Array<TypeArgument>& m_typeArguments;
    };
};