#pragma once
#include <compile/types.h>
#include <utils/Array.h>

namespace bind {
    class DataType;
    class FunctionType;
};

namespace compile {
    class Context;
    class TypeArgument;
    class TemplateFunction;

    class FunctionSpecializer {
        public:
            void setReturnType(bind::DataType* type);
            void setThisType(bind::DataType* type);
            void setArgumentType(u32 idx, bind::DataType* type);

            const Array<TypeArgument>& getTypeArguments() const;

        protected:
            friend class TemplateFunction;

            FunctionSpecializer(
                TemplateFunction* templ,
                Context* compilationContext,
                const Array<TypeArgument>& typeArgs
            );
            virtual ~FunctionSpecializer();

            TemplateFunction* m_func;
            bind::DataType* m_returnType;
            bind::DataType* m_thisType;
            Array<bind::DataType*> m_argumentTypes;
            const Array<TypeArgument>& m_typeArguments;
            
            bind::FunctionType* generateSignature(bool* didExist);
    };
};