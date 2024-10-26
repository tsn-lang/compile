#pragma once
#include <compile/types.h>
#include <bind/DataType.h>
#include <utils/String.h>
#include <utils/Array.h>

namespace compile {
    class Context;
    class TypeArgument;
    class TemplateType;

    class TypeSpecializer {
        public:
            bind::DataType::Property& addProperty(const String& name, bind::DataType* type);

            const Array<TypeArgument>& getTypeArguments() const;
        
        protected:
            friend class Context;

            TypeSpecializer(TemplateType* templ, Context* compilationContext, const Array<TypeArgument>& typeArgs);
            virtual ~TypeSpecializer();

            bind::DataType* generateType();

            TemplateType* m_specializedType;
            Context* m_compilation;
            bind::type_meta m_meta;
            Array<bind::DataType::Property> m_properties;
            const Array<TypeArgument>& m_typeArguments;
    };
};