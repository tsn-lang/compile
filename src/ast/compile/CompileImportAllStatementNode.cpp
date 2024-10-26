#include <compile/ast/CompileHandler.h>
#include <compile/Context.h>
#include <compile/CompiledFunction.h>
#include <compile/interfaces/IModuleResolver.h>
#include <compile/Scope.h>
#include <compile/SymbolTable.h>
#include <compile/Symbol.h>
#include <compile/Module.h>
#include <compile/Compiler.h>
#include <parse/statements/ImportAllStatementNode.h>
#include <parse/literals/StringLiteralNode.h>
#include <parse/misc/IdentifierNode.h>
#include <utils/Array.hpp>

using namespace parse;
using namespace bind;
using namespace codegen;

namespace compile {
    void CompileHandler::visit(ImportAllStatementNode* node) {
        m_ctx->enterNode(node->moduleId);
        Module* mod = m_ctx->getCompiler()->getModuleResolver()->resolve(node->moduleId->value, m_ctx);
        m_ctx->exitNode();

        if (!mod) return;

        m_ctx->enterNode(node);
        
        if (node->alias) {
            Symbol* s = m_ctx->getGlobalScope()->getSymbols().add(node->alias->text, mod);
            m_ctx->mapSymbol(s, node->alias->getLocation());
        } else {
            const Array<Symbol*>& symbols = mod->getSymbols();
            SymbolTable& global = m_ctx->getGlobalScope()->getSymbols();
            for (u32 i = 0;i < symbols.size();i++) {
                Symbol* s = symbols[i];
                global.add(s->getName(), symbols[i]);
                m_ctx->addImport(s);
            }
        }

        m_ctx->exitNode();
    }
};