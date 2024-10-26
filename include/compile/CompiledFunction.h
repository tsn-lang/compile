#pragma once
#include <compile/types.h>
#include <codegen/FunctionBuilder.h>

namespace compile {
    class Context;
    class Scope;

    class CompiledFunction {
        public:
            CompiledFunction(Context* ctx, bind::Function* func);
            ~CompiledFunction();

            void init();

            Scope* getRootScope() const;
            Scope* getCurrentScope() const;
            codegen::FunctionBuilder* getBuilder();

        protected:
            friend class Scope;

            Context* m_ctx;
            Scope* m_rootScope;
            Array<Scope*> m_scopeStack;
            codegen::FunctionBuilder m_builder;
    };
};