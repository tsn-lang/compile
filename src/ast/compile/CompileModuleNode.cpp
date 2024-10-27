#include <compile/ast/CompileHandler.h>
#include <compile/Context.h>
#include <compile/CompiledFunction.h>
#include <codegen/FunctionBuilder.h>
#include <compile/CompiledFunction.h>
#include <compile/Module.h>
#include <parse/misc/ModuleNode.h>
#include <parse/proxies/TopLevelNode.h>
#include <bind/Function.h>
#include <bind/Registry.hpp>
#include <utils/Array.hpp>

using namespace parse;
using namespace bind;
using namespace codegen;

namespace compile {
    void CompileHandler::visit(ModuleNode* node) {
        CompiledFunction* cf = m_ctx->getCurrentFunction();
        FunctionBuilder* fb = cf->getBuilder();
        m_ctx->enterNode(node);

        bind::FunctionType* initFnSig = bind::Registry::Signature<void>();
        bind::Function* initFn = new bind::Function("$init", initFnSig, m_ctx->getModule()->getNamespace());
        m_ctx->onFunctionGenerated(initFn);

        CompiledFunction* init = new CompiledFunction(m_ctx, initFn);

        m_ctx->enterFunction(init);
        m_ctx->importGlobals();

        for (u32 i = 0;i < node->statements.size();i++) {
            parse::TopLevelNode* n = node->statements[i];
            n->acceptVisitor(this);
        }

        m_ctx->exitFunction();
        m_ctx->exitNode();
    }
};