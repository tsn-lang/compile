#include <compile/TypeSpecializer.h>
#include <compile/TemplateType.h>
#include <compile/TypeArgument.h>
#include <compile/TemplateFunction.h>
#include <bind/Namespace.h>
#include <utils/interfaces/IWithUserData.hpp>
#include <utils/Array.hpp>

namespace compile {
    TypeSpecializer::TypeSpecializer(TemplateType* templ, Context* compilationContext, const Array<TypeArgument>& typeArgs) 
        : m_specializedType(templ), m_generatedType(nullptr), m_compilation(compilationContext),
        m_typeArguments(typeArgs), m_hasDtor(false)
    {
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

        struct dummy {
            u8 pad;
        };
        
        bind::Namespace* ns = m_specializedType->getOwnNamespace()->getParent();
        m_generatedType = new bind::DataType(name, fullName, bind::meta<dummy>(), ns);

        // type metadata going into DataType constructor must have size > 0 for ffi type purposes
        // but it needs to be 0 from here on
        bind::ITypeBuilder(m_generatedType).getMeta().size = 0;
    }

    TypeSpecializer::~TypeSpecializer() {
        if (m_generatedType) {
            bind::Registry::Remove(m_generatedType);
            delete m_generatedType;
            m_generatedType = nullptr;
        }
    }

    bind::DataType::Property& TypeSpecializer::addProperty(const String& name, bind::DataType* type) {
        bind::DataType::Property::Flags flags;
        memset(&flags, 0, sizeof(flags));
        flags.can_read = 1;
        flags.can_write = 1;

        bind::ITypeBuilder ext(m_generatedType);
        bind::type_meta& meta = ext.getMeta();

        u32 offset = meta.size;
        meta.size += type->getInfo().size;
        bind::DataType::Property& prop = ext.addProperty(offset, flags, type, name);

        tsn_property_userdata& data = prop.getUserData<tsn_property_userdata>();
        data.flags.indicator_bit = 1;

        return prop;
    }

    void TypeSpecializer::addMethod(TemplateFunction* method) {
        bind::DataType::Property::Flags f;
        memset(&f, 0, sizeof(f));
        f.can_read = 1;
        f.is_method = 1;

        
        bind::ITypeBuilder ext(m_generatedType);
        bind::DataType::Property& prop = ext.addProperty(
            Pointer(method),
            f,
            nullptr,
            method->getName()
        );

        tsn_property_userdata& data = prop.getUserData<tsn_property_userdata>();
        data.flags.indicator_bit = 1;
        data.flags.is_host_template_method = 1;
    }

    void TypeSpecializer::addStaticMethod(TemplateFunction* method) {
        bind::DataType::Property::Flags f;
        memset(&f, 0, sizeof(f));
        f.can_read = 1;
        f.is_method = 1;
        f.is_static = 1;
        
        bind::ITypeBuilder ext(m_generatedType);
        bind::DataType::Property& prop = ext.addProperty(
            Pointer(method),
            f,
            nullptr,
            method->getName()
        );

        tsn_property_userdata& data = prop.getUserData<tsn_property_userdata>();
        data.flags.indicator_bit = 1;
        data.flags.is_host_template_method = 1;
    }

    const Array<TypeArgument>& TypeSpecializer::getTypeArguments() const {
        return m_typeArguments;
    }
    
    bind::DataType* TypeSpecializer::getSpecializedType() const {
        return m_generatedType;
    }
    
    Context* TypeSpecializer::getCompilationContext() const {
        return m_compilation;
    }

    bind::DataType* TypeSpecializer::claimType() {
        bind::DataType* tp = m_generatedType;
        m_generatedType = nullptr;
        return tp;
    }
};