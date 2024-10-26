#pragma once
#include <compile/types.h>
#include <utils/String.h>
#include <utils/Array.h>

namespace codegen {
    class Value;
};

namespace bind {
    class DataType;
    class Function;
};

namespace compile {
    class Module;
    class Symbol;

    class SymbolTable {
        public:
            SymbolTable();
            ~SymbolTable();

            Symbol* add(const String& name, const codegen::Value& value);
            Symbol* add(const String& name, bind::DataType* type);
            Symbol* add(const String& name, bind::Function* func);
            Symbol* add(const String& name, Module* mod);
            Symbol* add(const String& name, Symbol* mod);

            void findSymbols(const String& name, Array<Symbol*>& out) const;
    };
};