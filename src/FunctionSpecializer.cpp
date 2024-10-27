#include <compile/FunctionSpecializer.h>
#include <compile/TemplateFunction.h>
#include <bind/Registry.h>
#include <utils/Exception.h>

namespace compile {
    FunctionSpecializer::FunctionSpecializer(
        TemplateFunction* templ,
        Context* compilationContext,
        const Array<TypeArgument>& typeArgs
    ) : m_func(templ), m_returnType(nullptr), m_thisType(nullptr), m_argumentTypes(templ->getArgumentCount()),
        m_typeArguments(typeArgs)
    {
        for (u32 i = 0;i < m_func->getArgumentCount();i++) {
            m_argumentTypes.push(nullptr);
        }
    }

    FunctionSpecializer::~FunctionSpecializer() {
    }

    void FunctionSpecializer::setReturnType(bind::DataType* type) {
        m_returnType = type;
    }

    void FunctionSpecializer::setThisType(bind::DataType* type) {
        m_thisType = type;
    }

    void FunctionSpecializer::setArgumentType(u32 idx, bind::DataType* type) {
        if (idx >= m_argumentTypes.size()) {
            throw Exception("FunctionSpecializer::setArgumentType - specified index out of range");
        }

        m_argumentTypes[idx] = type;
    }

    const Array<TypeArgument>& FunctionSpecializer::getTypeArguments() const {
        return m_typeArguments;
    }
            
    bind::FunctionType* FunctionSpecializer::generateSignature(bool* didExist) {
        if (!m_returnType) {
            throw Exception("FunctionSpecializer::generateSignature - return type has not been provided");
        }

        for (u32 i = 0;i < m_argumentTypes.size();i++) {
            if (!m_argumentTypes[i]) {
                throw Exception("FunctionSpecializer::generateSignature - type of argument %d has not been provided", i);
            }
        }

        if (m_thisType) {
            return bind::Registry::MethodSignature(m_returnType, m_thisType, m_argumentTypes.data(), m_argumentTypes.size(), didExist);
        }

        return bind::Registry::Signature(m_returnType, m_argumentTypes.data(), m_argumentTypes.size(), didExist);
    }
};