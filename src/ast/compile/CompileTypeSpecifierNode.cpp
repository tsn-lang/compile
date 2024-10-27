#include <compile/ast/CompileHandler.h>
#include <compile/Context.h>
#include <compile/CompiledFunction.h>
#include <compile/types/BuiltinArray.h>
#include <codegen/FunctionBuilder.h>
#include <parse/types/TypeSpecifierNode.h>
#include <bind/Registry.hpp>
#include <utils/Array.hpp>

using namespace parse;
using namespace bind;
using namespace codegen;

namespace compile {
    void CompileHandler::visit(TypeSpecifierNode* node) {
        CompiledFunction* cf = m_ctx->getCurrentFunction();
        FunctionBuilder* fb = cf->getBuilder();
        m_ctx->enterNode(node);

        if (node->mod == TypeSpecifierNode::Modifier::Pointer) {
            node->type->acceptVisitor(this);
            bind::DataType* tp = m_resolvedTypeStack.pop();
            m_resolvedTypeStack.push(tp->getPointerType());
        } else if (node->mod == TypeSpecifierNode::Modifier::Array) {
            TemplateType* arr = (TemplateType*)bind::Registry::GetType<array_t>();
            bind::DataType* tp = arr->specialize(m_ctx, this, { node->type });
            m_resolvedTypeStack.push(tp);
        } else {
            node->type->acceptVisitor(this);
        }

        m_ctx->exitNode();
    }
};