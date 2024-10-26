#include <compile/ast/CompileHandler.h>
#include <compile/Context.h>
#include <compile/CompiledFunction.h>
#include <codegen/FunctionBuilder.h>
#include <parse/misc/TypedAssignableNode.h>

using namespace parse;
using namespace bind;
using namespace codegen;

namespace compile {
    void CompileHandler::visit(TypedAssignableNode* node) {
        CompiledFunction* cf = m_ctx->getCurrentFunction();
        FunctionBuilder* fb = cf->getBuilder();
        m_ctx->enterNode(node);
        m_ctx->exitNode();
    }
};