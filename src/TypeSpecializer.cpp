#include <compile/TypeSpecializer.h>
#include <compile/TemplateType.h>
#include <compile/TypeArgument.h>
#include <bind/Namespace.h>
#include <utils/Array.hpp>

namespace compile {
    TypeSpecializer::TypeSpecializer(TemplateType* templ, Context* compilationContext, const Array<TypeArgument>& typeArgs) 
        : m_specializedType(templ), m_compilation(compilationContext), m_typeArguments(typeArgs)
    {
    }

    TypeSpecializer::~TypeSpecializer() {
    }

    bind::DataType::Property& TypeSpecializer::addProperty(const String& name, bind::DataType* type) {
        bind::DataType::Property::Flags flags;
        memset(&flags, 0, sizeof(flags));
        flags.can_read = 1;
        flags.can_write = 1;

        m_properties.emplace(m_meta.size, flags, type, name);
        m_meta.size += type->getInfo().size;

        return m_properties.last();
    }

    const Array<TypeArgument>& TypeSpecializer::getTypeArguments() const {
        return m_typeArguments;
    }

    bind::DataType* TypeSpecializer::generateType() {
        String name = m_specializedType->getName() + '<';
        String fullName = m_specializedType->getFullName() + '<';
        
        for (u32 i = 0;i < m_typeArguments.size();i++) {
            const TypeArgument& arg = m_typeArguments[i];
            if (i > 0) {
                name += ',';
                fullName += ',';
            }

            name += arg.getType()->getName();
            fullName += arg.getType()->getFullName();
        }

        name += '>';
        fullName += '>';
        
        bind::Namespace* ns = m_specializedType->getOwnNamespace()->getParent();
        bind::DataType* tp = new bind::DataType(name, fullName, m_meta, ns);
        return tp;
    }
};