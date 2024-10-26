#include <compile/ast/CompileHandler.h>
#include <compile/Context.h>
#include <compile/CompiledFunction.h>
#include <compile/interfaces/IModuleResolver.h>
#include <compile/Scope.h>
#include <compile/SymbolTable.h>
#include <compile/Symbol.h>
#include <compile/Module.h>
#include <compile/Compiler.h>
#include <parse/statements/ImportSelectStatementNode.h>
#include <parse/literals/StringLiteralNode.h>
#include <parse/misc/IdentifierNode.h>
#include <utils/Array.hpp>

using namespace parse;
using namespace bind;
using namespace codegen;

namespace compile {
    void CompileHandler::visit(ImportSelectStatementNode* node) {
        m_ctx->enterNode(node->moduleId);
        Module* mod = m_ctx->getCompiler()->getModuleResolver()->resolve(node->moduleId->value, m_ctx);
        m_ctx->exitNode();

        if (!mod) return;
        m_ctx->addDependency(mod);

        m_ctx->enterNode(node);
        
        SymbolTable& global = m_ctx->getGlobalScope()->getSymbols();
        for (u32 i = 0;i < node->importList.size();i++) {
            parse::IdentifierNode* name = node->importList[i];
            m_ctx->enterNode(name);

            utils::Array<Symbol*> symbols;
            mod->findSymbols(name->text, symbols);

            if (symbols.size() == 0) {
                m_ctx->logError("Module '%s' has no symbols named '%s'", mod->getNamespace()->getName().c_str(), name->text.c_str());
            } else {
                for (u32 i = 0;i < symbols.size();i++) {
                    Symbol* s = symbols[i];
                    global.add(s->getName(), symbols[i]);
                    m_ctx->addImport(s);
                    m_ctx->mapSymbol(s, name->getLocation());
                }
            }

            m_ctx->exitNode();
        }

        m_ctx->exitNode();
    }
};