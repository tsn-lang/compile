#include <compile/Context.h>
#include <compile/CompiledFunction.h>
#include <compile/Module.h>
#include <compile/interfaces/IModuleSource.h>
#include <compile/interfaces/IModuleFactory.h>
#include <compile/ast/CompileHandler.h>
#include <compile/TypeArgument.h>
#include <compile/TemplateType.h>
#include <compile/TypeSpecializer.h>
#include <compile/TemplateFunction.h>
#include <compile/FunctionSpecializer.h>
#include <compile/Compiler.h>
#include <compile/Symbol.h>
#include <parse/misc/Node.h>
#include <parse/misc/ModuleNode.h>
#include <parse/types/TypeSpecifierNode.h>
#include <parse/Context.h>
#include <parse/Tokens.h>
#include <tokenize/Resource.h>
#include <tokenize/SourceException.h>
#include <tokenize/TokenSet.h>
#include <tokenize/TokenizedSource.h>
#include <bind/Registry.hpp>
#include <utils/Array.hpp>
#include <utils/interfaces/IWithUserData.hpp>

namespace compile {
    Context::Context(
        Compiler* compiler,
        IModuleSource* source
    ) : m_scopePool(sizeof(Scope), 64, false) {
        m_compiler = compiler;
        m_source = source;
        m_module = nullptr;
        m_currentFunction = nullptr;
    }

    Context::~Context() {
        if (didError()) discardOutput();

        for (u32 i = 0;i < m_allScopes.size();i++) {
            m_allScopes[i]->~Scope();
        }

        for (u32 i = 0;i < m_compiledFunctions.size();i++) {
            delete m_compiledFunctions[i];
        }

        for (u32 i = 0;i < m_allSymbols.size();i++) {
            delete m_allSymbols[i];
        }
        m_allSymbols.clear(true);
    }

    bool Context::compile() {
        m_module = m_compiler->getModuleFactory()->createModule(this);

        try {
            tokenize::TokenSet ts;
            parse::setupTokenSet(&ts);

            tokenize::TokenizedSource* tokens = m_source->getResource().tokenize(&ts);
            parse::Context pctx(tokens);
            
            parse::ModuleNode* m = parse::ModuleNode::TryParse(&pctx);

            for (u32 i = 0;i < pctx.getState()->messages.size();i++) {
                parse::ParseMessage& pm = pctx.getState()->messages[i];
                CompileMessage::Type tp;
                switch (pm.type) {
                    case parse::ParseMessage::Type::Debug: { tp = CompileMessage::Type::Debug; break; }
                    case parse::ParseMessage::Type::Info: { tp = CompileMessage::Type::Info; break; }
                    case parse::ParseMessage::Type::Warning: { tp = CompileMessage::Type::Warning; break; }
                    case parse::ParseMessage::Type::Error: { tp = CompileMessage::Type::Error; break; }
                }

                m_logs.push({
                    tp,
                    pm.message,
                    pm.location,
                    nullptr
                });
            }

            if (pctx.didError()) {
                logError("One or more syntax errors were encountered. Compilation aborted");
            } else if (m) {
                CompileHandler ch(this);
                m->acceptVisitor(&ch);
            }
        } catch (const tokenize::SourceException& exc) {
            logError(exc.what());
        }

        return !didError();
    }

    void Context::discardOutput() {
        for (u32 i = 0;i < m_generatedTypes.size();i++) {
            bind::Registry::Remove(m_generatedTypes[i]);
            delete m_generatedTypes[i];
        }
        m_generatedTypes.clear(true);

        if (m_module) m_compiler->getModuleFactory()->destroyModule(m_module, this);
        m_module = nullptr;
    }

    void Context::enterNode(parse::Node* node) {
        m_nodeStack.push(node);
    }

    void Context::exitNode() {
        m_nodeStack.pop();
    }

    void Context::enterFunction(CompiledFunction* func) {
        m_compiledFunctions.push(func);
        m_curFuncStack.push(func);
        m_currentFunction = func;
        func->init();
    }

    void Context::exitFunction() {
        m_currentFunction->getRootScope()->escape();
        m_curFuncStack.pop();
        m_currentFunction = m_curFuncStack.last();
    }

    Scope* Context::enterScope(bool isExplicit) {
        Scope* s = new (m_scopePool.alloc()) Scope(this, isExplicit);
        m_allScopes.push(s);
        return s;
    }

    void Context::addDependency(Module* mod) {
        m_dependencies.push(mod);
    }

    void Context::addImport(Symbol* sym) {
        m_imports.push(sym);
    }

    void Context::mapSymbol(Symbol* sym, const SourceLocation& loc) {
        m_symbolMap.push({ sym, loc });
    }
    
    void Context::onFunctionGenerated(bind::Function* func) {
        tsn_func_userdata& data = func->getUserData<tsn_func_userdata>();
        data.flags.indicator_bit = 1;
        m_generatedFunctions.push(func);
    }

    void Context::onTypeGenerated(bind::DataType* type) {
        tsn_type_userdata& data = type->getUserData<tsn_type_userdata>();
        data.flags.indicator_bit = 1;
        m_generatedTypes.push(type);
    }

    void Context::onSymbolGenerated(Symbol* sym) {
        m_allSymbols.push(sym);
    }
    
    Module* Context::getModule() const {
        return m_module;
    }

    Scope* Context::getGlobalScope() const {
        return m_compiledFunctions[0]->getRootScope();
    }

    Compiler* Context::getCompiler() const {
        return m_compiler;
    }

    IModuleSource* Context::getModuleSource() const {
        return m_source;
    }

    CompiledFunction* Context::getInitFunction() const {
        return m_compiledFunctions[0];
    }

    CompiledFunction* Context::getCurrentFunction() const {
        return m_currentFunction;
    }

    const Array<Module*>& Context::getDependencies() const {
        return m_dependencies;
    }

    const Array<Symbol*>& Context::getImportedSymbols() const {
        return m_imports;
    }

    const Array<SymbolMapEntry>& Context::getSymbolMap() const {
        return m_symbolMap;
    }

    const Array<CompileMessage>& Context::getLogs() const {
        return m_logs;
    }

    Array<CompileMessage>& Context::getLogs() {
        return m_logs;
    }

    void Context::onDebug(const char* msg) {
        m_logs.push({
            CompileMessage::Type::Debug,
            msg,
            m_nodeStack.last()->getLocation(),
            m_currentFunction
        });
    }

    void Context::onInfo(const char* msg) {
        m_logs.push({
            CompileMessage::Type::Info,
            msg,
            m_nodeStack.last()->getLocation(),
            m_currentFunction
        });
    }

    void Context::onWarn(const char* msg) {
        m_logs.push({
            CompileMessage::Type::Warning,
            msg,
            m_nodeStack.last()->getLocation(),
            m_currentFunction
        });
    }

    void Context::onError(const char* msg) {
        m_logs.push({
            CompileMessage::Type::Error,
            msg,
            m_nodeStack.last()->getLocation(),
            m_currentFunction
        });
    }

    void Context::importGlobals() {
        std::shared_lock l(bind::Registry::ReadLock());

        bind::Namespace* global = bind::Registry::GlobalNamespace();
        const Array<bind::ISymbol*>& symbols = global->getSymbols();

        SymbolTable& globalSymbols = getGlobalScope()->getSymbols();

        for (u32 i = 0;i < symbols.size();i++) {
            bind::ISymbol* sym = symbols[i];
            switch (sym->getSymbolType()) {
                case bind::SymbolType::Namespace: break;
                case bind::SymbolType::Function: {
                    Symbol* s = globalSymbols.add(sym->getName(), (bind::Function*)sym);
                    addImport(s);
                    break;
                }
                case bind::SymbolType::DataType: {
                    Symbol* s = globalSymbols.add(sym->getName(), (bind::DataType*)sym);
                    addImport(s);
                    break;
                }
                case bind::SymbolType::Value: {
                    Symbol* s = globalSymbols.add(sym->getName(), (bind::ValuePointer*)sym);
                    addImport(s);
                    break;
                }
            }
        }
    }
};