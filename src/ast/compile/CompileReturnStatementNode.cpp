#include <compile/ast/CompileHandler.h>
#include <compile/Context.h>
#include <compile/CompiledFunction.h>
#include <codegen/FunctionBuilder.h>
#include <parse/statements/ReturnStatementNode.h>

using namespace parse;
using namespace bind;
using namespace codegen;

namespace compile {
    void CompileHandler::visit(ReturnStatementNode* node) {
        CompiledFunction* cf = m_ctx->getCurrentFunction();
        FunctionBuilder* fb = cf->getBuilder();
        m_ctx->enterNode(node);
        m_ctx->exitNode();
    }
};