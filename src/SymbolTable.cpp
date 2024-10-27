#include <compile/SymbolTable.h>
#include <compile/Symbol.h>
#include <compile/Context.h>

namespace compile {
    SymbolTable::SymbolTable(Context* ctx) : m_ctx(ctx) {
    }

    SymbolTable::~SymbolTable() {
    }

    Symbol* SymbolTable::add(const String& name, const codegen::Value& value) {
        Symbol* s = new Symbol(value);
        m_ctx->onSymbolGenerated(s);
        return s;
    }

    Symbol* SymbolTable::add(const String& name, bind::DataType* type) {
        Symbol* s = new Symbol(type);
        m_ctx->onSymbolGenerated(s);
        return s;
    }

    Symbol* SymbolTable::add(const String& name, bind::Function* func) {
        Symbol* s = new Symbol(func);
        m_ctx->onSymbolGenerated(s);
        return s;
    }
    
    Symbol* SymbolTable::add(const String& name, bind::ValuePointer* global) {
        Symbol* s = new Symbol(global);
        m_ctx->onSymbolGenerated(s);
        return s;
    }

    Symbol* SymbolTable::add(const String& name, Module* mod) {
        Symbol* s = new Symbol(mod);
        m_ctx->onSymbolGenerated(s);
        return s;
    }

    Symbol* SymbolTable::add(const String& name, Symbol* sym) {
        return sym;
    }

    void SymbolTable::findSymbols(const String& name, Array<Symbol*>& out) const {
    }
};