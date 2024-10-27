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
#include <utils/interfaces/IWithUserData.hpp>

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
            tsn_type_userdata& data = tp->getUserData<tsn_type_userdata>();

            if (node->parameters.size() == 0) {
                if (data.flags.indicator_bit == 1 && data.flags.is_host_template == 1) {
                    TemplateType* templ = (TemplateType*)tp;
                    m_ctx->logError(
                        "Type '%s' expects %d type %s but none were provided",
                        node->name->text.c_str(),
                        templ->getTypeArgumentCount(),
                        templ->getTypeArgumentCount() == 1 ? "argument" : "arguments"
                    );
                    m_resolvedTypeStack.push(poison);
                } else {
                    m_resolvedTypeStack.push(tp);
                }
            } else {
                if (data.flags.indicator_bit == 0 || data.flags.is_host_template == 0) {
                    m_ctx->logError("Type '%s' is not a template", node->name->text.c_str());
                    m_resolvedTypeStack.push(poison);
                } else {
                    TemplateType* templ = (TemplateType*)tp;

                    if (node->parameters.size() != templ->getTypeArgumentCount()) {
                        m_ctx->logError(
                            "Type '%s' expects %d type %s but %d %s provided",
                            node->name->text.c_str(),
                            templ->getTypeArgumentCount(),
                            templ->getTypeArgumentCount() == 1 ? "argument" : "arguments",
                            node->parameters.size(),
                            node->parameters.size() == 1 ? "was" : "were"
                        );
                        m_resolvedTypeStack.push(poison);
                    } else {
                        bind::DataType* tp = templ->specialize(m_ctx, this, node->parameters.cast<parse::Node*>());
                        if (!tp) tp = poison;
                        m_resolvedTypeStack.push(tp);
                    }
                }
            }
        }

        m_ctx->exitNode();
    }
};