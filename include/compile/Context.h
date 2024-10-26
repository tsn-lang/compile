#pragma once
#include <compile/types.h>
#include <compile/Scope.h>
#include <tokenize/SourceLocation.h>
#include <utils/interfaces/ILogHandler.h>
#include <utils/interfaces/IWithLogging.h>
#include <utils/Array.h>
#include <utils/MemoryPool.h>

namespace bind {
    class Function;
    class DataType;
};

namespace parse {
    class Node;
};

namespace compile {
    class Compiler;
    class IModuleSource;
    class CompiledFunction;
    class TemplateType;
    class CompileHandler;
    
    struct CompileMessage {
        enum class Type {
            Debug,
            Info,
            Warning,
            Error
        };

        Type type;
        String message;
        SourceLocation location;
        CompiledFunction* function;
    };

    struct SymbolMapEntry {
        Symbol* symbol;
        SourceLocation location;
    };

    class Context : private ILogHandler, public IWithLogging {
        public:
            Context(
                Compiler* compiler,
                IModuleSource* source
            );
            ~Context();

            bool compile();
            void discardOutput();

            void enterNode(parse::Node* node);
            void exitNode();
            void enterFunction(CompiledFunction* func);
            void exitFunction();
            Scope* enterScope(bool isExplicit);

            void addDependency(Module* mod);
            void addImport(Symbol* sym);
            void mapSymbol(Symbol* sym, const SourceLocation& loc);

            void onFunctionCompiled(CompiledFunction* cf);
            void onFunctionGenerated(bind::Function* func);
            void onTypeGenerated(bind::DataType* type);
            bind::DataType* specializeType(CompileHandler* ch, TemplateType* templ, const Array<parse::Node*>& typeArgs);
            
            Module* getModule() const;
            Scope* getGlobalScope() const;
            Compiler* getCompiler() const;
            IModuleSource* getModuleSource() const;
            CompiledFunction* getInitFunction() const;
            CompiledFunction* getCurrentFunction() const;
            const Array<Module*>& getDependencies() const;
            const Array<Symbol*>& getImportedSymbols() const;
            const Array<SymbolMapEntry>& getSymbolMap() const;
            const Array<CompileMessage>& getLogs() const;
            Array<CompileMessage>& getLogs();
        
        protected:
            virtual void onDebug(const char* msg);
            virtual void onInfo(const char* msg);
            virtual void onWarn(const char* msg);
            virtual void onError(const char* msg);

            Compiler* m_compiler;
            IModuleSource* m_source;
            MemoryPool m_scopePool;
            Array<parse::Node*> m_nodeStack;
            Array<CompiledFunction*> m_compiledFunctions;
            Array<CompiledFunction*> m_curFuncStack;
            CompiledFunction* m_currentFunction;

            // outputs

            Module* m_module;
            Array<Module*> m_dependencies;
            Array<Symbol*> m_imports;
            Array<Scope*> m_allScopes;
            Array<SymbolMapEntry> m_symbolMap;
            Array<CompileMessage> m_logs;
            Array<bind::Function*> m_generatedFunctions;
            Array<bind::DataType*> m_generatedTypes;
    };
};