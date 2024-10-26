#include <compile/ast/CompileHandler.h>
#include <compile/Context.h>
#include <compile/CompiledFunction.h>
#include <compile/Symbol.h>
#include <compile/TemplateType.h>
#include <codegen/FunctionBuilder.h>
#include <parse/misc/IdentifierNode.h>
#include <parse/types/IdentifierTypeSpecifierNode.h>
#include <parse/types/TypeSpecifierNode.h>
#include <bind/Registry.hpp>
#include <utils/Array.hpp>

using namespace parse;
using namespace bind;
using namespace codegen;

namespace compile {
    void CompileHandler::visit(IdentifierTypeSpecifierNode* node) {
        bind::DataType* poison = bind::Registry::GetType<poison_t>();
        CompiledFunction* cf = m_ctx->getCurrentFunction();
        FunctionBuilder* fb = cf->getBuilder();

        m_ctx->enterNode(node->name);
        Array<Symbol*> symbols;
        cf->getCurrentScope()->findSymbols(node->name->text, symbols);
        
        DataType* tp = nullptr;

        if (symbols.size() == 0) {
            m_ctx->logError("Undefined identifier '%s'", node->name->text.c_str());
            m_resolvedTypeStack.push(poison);
        } else {
            for (Symbol* s : symbols) {
                if (s->getType() != Symbol::Type::DataType) continue;

                if (tp) {
                    m_ctx->logError("Ambiguous type name '%s'", node->name->text.c_str());
                    m_resolvedTypeStack.push(poison);
                    tp = nullptr;
                    break;
                } else {
                    tp = s->getValue().type;
                }
            }

            if (!tp) {
                m_ctx->logError("Identifier '%s' does not name a type", node->name->text.c_str());
                m_resolvedTypeStack.push(poison);
            }
        }

        if (tp) {
            if (node->parameters.size() == 0) {
                // todo: check if tp is a template, error if so
                m_resolvedTypeStack.push(tp);
            } else {
                // todo: check if tp is a template, error if not
                TemplateType* templ = nullptr;

                if (node->parameters.size() != templ->getTypeArgumentCount()) {
                    m_ctx->logError(
                        "Type '%s' expects %d %s but %d %s provided",
                        node->name->text.c_str(),
                        templ->getTypeArgumentCount(),
                        templ->getTypeArgumentCount() == 1 ? "argument" : "arguments",
                        node->parameters.size(),
                        node->parameters.size() == 1 ? "was" : "were"
                    );
                    m_resolvedTypeStack.push(poison);
                } else {
                    bind::DataType* tp = m_ctx->specializeType(this, templ, node->parameters.cast<parse::Node*>());
                    m_resolvedTypeStack.push(tp);
                }
            }
        }

        m_ctx->exitNode();
    }
};