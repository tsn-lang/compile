#pragma once
#include <compile/TypeSpecializer.h>
#include <bind/Registry.hpp>
#include <utils/Array.hpp>

namespace compile {
    template <typename TemplateTypeCls>
    void TypeSpecializer::addConstructor(
        bind::FunctionType* signature,
        void (TemplateTypeCls::*methodPtr)(codegen::FunctionBuilder*)
    ) {
        if (signature->getReturnType() != bind::Registry::GetType<void>()) {
            throw Exception("TypeSpecializer::addConstructor - constructor should have return type 'void'");
        }

        if (signature->getThisType() != m_generatedType) {
            throw Exception(
                "TypeSpecializer::addConstructor - constructor should have 'this' type '%s'",
                m_generatedType->getName().c_str()
            );
        }

        bind::DataType::Property::Flags f;
        memset(&f, 0, sizeof(f));
        f.can_read = 1;
        f.is_ctor = 1;

        m_methods.push({
            bind::ConstructorName,
            f,
            signature,
            (void (TemplateType::*)(codegen::FunctionBuilder*))methodPtr
        });
    }

    template <typename TemplateTypeCls>
    void TypeSpecializer::addDestructor(
        bind::FunctionType* signature,
        void (TemplateTypeCls::*methodPtr)(codegen::FunctionBuilder*)
    ) {
        if (m_hasDtor) {
            throw Exception("TypeSpecializer::registerDestructor - destructor already defined");
        }
        m_hasDtor = true;
        
        if (signature->getReturnType() != bind::Registry::GetType<void>()) {
            throw Exception("TypeSpecializer::addDestructor - destructor should have return type 'void'");
        }

        if (signature->getThisType() != m_generatedType) {
            throw Exception(
                "TypeSpecializer::addDestructor - destructor should have 'this' type '%s'",
                m_generatedType->getName().c_str()
            );
        }

        if (signature->getArgs().size() > 0) {
            throw Exception("TypeSpecializer::addDestructor - destructor should have no arguments");
        }

        bind::DataType::Property::Flags f;
        memset(&f, 0, sizeof(f));
        f.can_read = 1;
        f.is_dtor = 1;

        m_methods.push({
            bind::DestructorName,
            f,
            signature,
            (void (TemplateType::*)(codegen::FunctionBuilder*))methodPtr
        });
    }

    template <typename TemplateTypeCls>
    void TypeSpecializer::addMethod(
        const String& name,
        bind::FunctionType* signature,
        void (TemplateTypeCls::*methodPtr)(codegen::FunctionBuilder*)
    ) {
        if (signature->getThisType() != m_generatedType) {
            throw Exception(
                "TypeSpecializer::addMethod - method '%s' should have 'this' type '%s'",
                name.c_str(),
                m_generatedType->getName().c_str()
            );
        }

        bind::DataType::Property::Flags f;
        memset(&f, 0, sizeof(f));
        f.can_read = 1;
        f.is_method = 1;

        m_methods.push({
            name,
            f,
            signature,
            (void (TemplateType::*)(codegen::FunctionBuilder*))methodPtr
        });
    }
    
    template <typename TemplateTypeCls>
    void TypeSpecializer::addStaticMethod(
        const String& name,
        bind::FunctionType* signature,
        void (TemplateTypeCls::*methodPtr)(codegen::FunctionBuilder*)
    ) {
        if (signature->getThisType() != nullptr) {
            throw Exception(
                "TypeSpecializer::addStaticMethod - method '%s' should have no 'this' type",
                name.c_str()
            );
        }

        bind::DataType::Property::Flags f;
        memset(&f, 0, sizeof(f));
        f.can_read = 1;
        f.is_method = 1;
        f.is_static = 1;

        m_methods.push({
            name,
            f,
            signature,
            (void (TemplateType::*)(codegen::FunctionBuilder*))methodPtr
        });
    }
};