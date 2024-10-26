#include <compile/CompiledFunction.h>
#include <compile/Context.h>
#include <utils/Array.hpp>

namespace compile {
    CompiledFunction::CompiledFunction(Context* ctx, bind::Function* func) : m_ctx(ctx), m_rootScope(nullptr), m_builder(func) {
    }

    CompiledFunction::~CompiledFunction() {
    }

    void CompiledFunction::init() {
        if (m_rootScope) return;
        m_rootScope = m_ctx->enterScope(true);
        m_scopeStack.push(m_rootScope);
    }

    Scope* CompiledFunction::getRootScope() const {
        return m_rootScope;
    }

    Scope* CompiledFunction::getCurrentScope() const {
        if (m_scopeStack.size() == 0) return m_rootScope;
        return m_scopeStack.last();
    }

    codegen::FunctionBuilder* CompiledFunction::getBuilder() {
        return &m_builder;
    }
};