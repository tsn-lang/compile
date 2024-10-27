#include <compile/Scope.h>
#include <compile/Context.h>
#include <compile/CompiledFunction.h>
#include <utils/Exception.h>
#include <utils/Array.hpp>

namespace compile {
    Scope::Scope(Context* ctx, bool isExplicit) :
        m_ctx(ctx),
        m_symbols(ctx),
        m_owner(ctx->getCurrentFunction()),
        m_parent(ctx->getCurrentFunction()->getCurrentScope()),
        m_cgScope(ctx->getCurrentFunction()->getBuilder()),
        m_isExplicit(isExplicit)
    {
        m_owner->m_scopeStack.push(this);
    }

    Scope::~Scope() {
        if (!m_cgScope.didEscape()) {
            // Encountered unescaped scope
            abort();
        }

        if (m_owner->m_scopeStack.last() != this) {
            // Compilation scopes not freed in the reverse of the order they were created in
            abort();
        }

        m_owner->m_scopeStack.pop();
    }

    SymbolTable& Scope::getSymbols() {
        return m_symbols;
    }

    void Scope::findSymbols(const String& name, utils::Array<Symbol*>& out) const {
        m_symbols.findSymbols(name, out);
        if (m_parent) m_parent->findSymbols(name, out);
    }

    const SourceLocation& Scope::getLocation() const {
        return m_location;
    }

    bool Scope::isExplicit() const {
        return m_isExplicit;
    }

    void Scope::escape() {
        m_cgScope.escape();
    }
    
    void Scope::escape(const codegen::Value& withValue) {
        m_cgScope.escape(withValue);
    }

    bool Scope::didEscape() const {
        return m_cgScope.didEscape();
    }

    void Scope::setLoopContinueLabel(codegen::label_id continueLbl) {
        m_cgScope.setLoopContinueLabel(continueLbl);
    }

    codegen::label_id Scope::getLoopContinueLabel() const {
        return m_cgScope.getLoopContinueLabel();
    }

    void Scope::loopContinue() {
        m_cgScope.loopContinue();
    }

    void Scope::setLoopBreakLabel(codegen::label_id breakLbl) {
        m_cgScope.setLoopBreakLabel(breakLbl);
    }
    
    codegen::label_id Scope::getLoopBreakLabel() const {
        return m_cgScope.getLoopBreakLabel();
    }

    void Scope::loopBreak() {
        m_cgScope.loopBreak();
    }
};