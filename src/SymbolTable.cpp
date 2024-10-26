#include <compile/SymbolTable.h>

namespace compile {
    SymbolTable::SymbolTable() {
    }

    SymbolTable::~SymbolTable() {
    }

    Symbol* SymbolTable::add(const String& name, const codegen::Value& value) {
        return nullptr;
    }

    Symbol* SymbolTable::add(const String& name, bind::DataType* type) {
        return nullptr;
    }

    Symbol* SymbolTable::add(const String& name, bind::Function* func) {
        return nullptr;
    }

    Symbol* SymbolTable::add(const String& name, Module* mod) {
        return nullptr;
    }

    Symbol* SymbolTable::add(const String& name, Symbol* mod) {
        return nullptr;
    }

    void SymbolTable::findSymbols(const String& name, Array<Symbol*>& out) const {
    }
};