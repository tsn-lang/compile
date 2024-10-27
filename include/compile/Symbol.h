#pragma once
#include <compile/types.h>
#include <codegen/Value.h>
#include <tokenize/SourceLocation.h>
#include <utils/String.h>

namespace bind {
    class DataType;
    class Function;
    class ValuePointer;
};

namespace compile {
    class Module;

    class Symbol {
        public:
            enum class Type : u8 {
                Module = 0,
                DataType,
                Function,
                ValuePointer,
                Variable
            };
            
            struct Value {
                Module* mod;
                bind::DataType* type;
                bind::Function* func;
                bind::ValuePointer* global;
                codegen::Value var;
            };

            Symbol(Module* mod);
            Symbol(bind::DataType* type);
            Symbol(bind::Function* func);
            Symbol(bind::ValuePointer* global);
            Symbol(const codegen::Value& var);
            ~Symbol();

            Type getType() const;
            const Value& getValue() const;
            bool hasDeclarationLocation() const;
            bool hasDefinitionLocation() const;
            bool hasLifetimeLocation() const;
            const SourceLocation& getDeclarationLocation() const;
            void setDeclarationLocation(const SourceLocation& loc);
            const SourceLocation& getDefinitionLocation() const;
            void setDefinitionLocation(const SourceLocation& loc);
            const SourceLocation& getLifetimeEndLocation() const;
            void setLifetimeEndLocation(const SourceLocation& loc);
        
        protected:
            Type m_type;
            Value m_value;
            bool m_hasDeclLoc;
            bool m_hasDefLoc;
            bool m_hasLifetimeLoc;
            SourceLocation m_declaredAt;
            SourceLocation m_definedAt;
            SourceLocation m_lifetimeEnd;
    };
};