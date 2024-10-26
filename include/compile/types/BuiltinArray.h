#pragma once
#include <compile/types.h>
#include <compile/TemplateType.h>

namespace bind {
    class DataType;
};

namespace compile {
    class BuiltinArray : public TemplateType {
        public:
            BuiltinArray();
            ~BuiltinArray();

            virtual bool onSpecialization(TypeSpecializer* specializer);

            void push(codegen::FunctionBuilder* fb);

            static bind::DataType* Register();
    };
};