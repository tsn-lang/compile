#pragma once
#include <compile/types.h>
#include <compile/SymbolTable.h>
#include <codegen/Scope.h>
#include <tokenize/SourceLocation.h>
#include <utils/Array.h>
#include <utils/String.h>


namespace compile {
    class Context;
    class CompiledFunction;
    class Symbol;

    class Scope {
        public:
            /**
             * @brief Gets the symbol table for this scope
             */
            SymbolTable& getSymbols();
            
            /**
             * @brief Finds a set of symbols with the given name that are visible within this scope.
             * Including symbols that are defined in parent scopes
             * 
             * @param name Symbol name to search for
             * @param out Output `Array` of symbols that are found
             */
            void findSymbols(const String& name, utils::Array<Symbol*>& out) const;

            /**
             * @brief Gets the ranged source location that this scope spans
             */
            const SourceLocation& getLocation() const;

            /**
             * @brief Whether or not this scope was explicitly defined in the code by a statement block
             */
            bool isExplicit() const;

            /**
             * @brief Escapes the current scope, destructing all the stack objects allocated within
             * it and freeing all the stack IDs. This can only be done once per scope instance.
             */
            void escape();
            
            /**
             * @brief Escapes the current scope, destructing all the stack objects allocated within
             * it and freeing all the stack IDs. This can only be done once per scope instance.
             * 
             * @param withValue Value to save from the stack cleanup. It will not be deconstructed
             * and its corresponding stack ID will not be freed. Additionally, the value and its
             * stack ID will be added to the parent scope
             */
            void escape(const codegen::Value& withValue);

            /**
             * @brief Whether or not this scope has escaped
             */
            bool didEscape() const;

            /**
             * @brief If this scope corresponds to a loop, this function will set the label that should
             * be jumped to when control should move back to the start of the loop
             * 
             * @param continueLbl Label to jump to when the control should move back to the start of the
             * loop
             */
            void setLoopContinueLabel(codegen::label_id continueLbl);

            /**
             * @brief If this scope or any parent scope corresponds to a loop, this function will get the
             * label that should be jumped to when control should move back to the start of the loop
             */
            codegen::label_id getLoopContinueLabel() const;

            /**
             * @brief If this scope or any parent scope corresponds to a loop, this function will emit IR
             * instructions that will cause control to move back to the start of the loop. This includes
             * stack cleanup instructions for all scopes between this one and the one that corresponds to
             * the loop
             */
            void loopContinue();

            /**
             * @brief If this scope corresponds to a loop, this function will set the label that should
             * be jumped to when control should break out of the loop and continue after it
             * 
             * @param breakLbl Label to jump to when the control should break out of the loop and continue
             * after it
             */
            void setLoopBreakLabel(codegen::label_id breakLbl);
            
            /**
             * @brief If this scope or any parent scope corresponds to a loop, this function will get the
             * label that should be jumped to when control should break out of the loop and continue after
             * it
             */
            codegen::label_id getLoopBreakLabel() const;

            /**
             * @brief If this scope or any parent scope corresponds to a loop, this function will emit IR
             * instructions that will cause control to break out of the loop and continue after it. This
             * includes stack cleanup instructions for all scopes between this one and the one that
             * corresponds to the loop
             */
            void loopBreak();
        
        protected:
            friend class Context;
            Scope(Context* ctx, bool isExplicit);
            ~Scope();

            Context* m_ctx;
            Scope* m_parent;
            CompiledFunction* m_owner;
            codegen::Scope m_cgScope;
            bool m_isExplicit;
            SymbolTable m_symbols;
            SourceLocation m_location;
    };
};