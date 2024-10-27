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

            utils::Array<ISymbol*> symbols;
            mod->getNamespace()->findSymbols(name->text, symbols);

            if (symbols.size() == 0) {
                m_ctx->logError("Module '%s' has no symbols named '%s'", mod->getNamespace()->getName().c_str(), name->text.c_str());
            } else {
                for (u32 i = 0;i < symbols.size();i++) {
                    bind::ISymbol* sym = symbols[i];
                    switch (sym->getSymbolType()) {
                        case bind::SymbolType::Namespace: break;
                        case bind::SymbolType::Function: {
                            Symbol* s = global.add(sym->getName(), (bind::Function*)sym);
                            m_ctx->addImport(s);
                            break;
                        }
                        case bind::SymbolType::DataType: {
                            Symbol* s = global.add(sym->getName(), (bind::DataType*)sym);
                            m_ctx->addImport(s);
                            break;
                        }
                        case bind::SymbolType::Value: {
                            Symbol* s = global.add(sym->getName(), (bind::ValuePointer*)sym);
                            m_ctx->addImport(s);
                            break;
                        }
                    }
                }
            }

            m_ctx->exitNode();
        }

        m_ctx->exitNode();
    }
};