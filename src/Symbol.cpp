#include <compile/Symbol.h>

namespace compile {
    Symbol::Symbol(Module* mod)
        : m_value({ mod, nullptr, nullptr, nullptr, codegen::Value() }), m_type(Symbol::Type::Module)
    {
        m_hasDeclLoc = false;
        m_hasDefLoc = false;
        m_hasLifetimeLoc = false;
    }

    Symbol::Symbol(bind::DataType* type)
        : m_value({ nullptr, type, nullptr, nullptr, codegen::Value() }), m_type(Symbol::Type::DataType)
    {
        m_hasDeclLoc = false;
        m_hasDefLoc = false;
        m_hasLifetimeLoc = false;
    }

    Symbol::Symbol(bind::Function* func)
        : m_value({ nullptr, nullptr, func, nullptr, codegen::Value() }), m_type(Symbol::Type::Function)
    {
        m_hasDeclLoc = false;
        m_hasDefLoc = false;
        m_hasLifetimeLoc = false;
    }
    
    Symbol::Symbol(bind::ValuePointer* global)
        : m_value({ nullptr, nullptr, nullptr, global, codegen::Value() }) , m_type(Symbol::Type::ValuePointer)
    {
        m_hasDeclLoc = false;
        m_hasDefLoc = false;
        m_hasLifetimeLoc = false;
    }

    Symbol::Symbol(const codegen::Value& var)
        : m_value({ nullptr, nullptr, nullptr, nullptr, var }), m_type(Symbol::Type::Variable)
    {
        m_hasDeclLoc = false;
        m_hasDefLoc = false;
        m_hasLifetimeLoc = false;
    }


    Symbol::~Symbol() {
    }

    
    Symbol::Type Symbol::getType() const {
        return m_type;
    }

    const Symbol::Value& Symbol::getValue() const {
        return m_value;
    }

    bool Symbol::hasDeclarationLocation() const {
        return m_hasDeclLoc;
    }

    bool Symbol::hasDefinitionLocation() const {
        return m_hasDefLoc;
    }

    bool Symbol::hasLifetimeLocation() const {
        return m_hasLifetimeLoc;
    }

    const SourceLocation& Symbol::getDeclarationLocation() const {
        return m_declaredAt;
    }
    
    void Symbol::setDeclarationLocation(const SourceLocation& loc) {
        m_declaredAt = loc;
        m_hasDeclLoc = true;
    }

    const SourceLocation& Symbol::getDefinitionLocation() const {
        return m_definedAt;
    }
    
    void Symbol::setDefinitionLocation(const SourceLocation& loc) {
        m_definedAt = loc;
        m_hasDefLoc = true;
    }

    const SourceLocation& Symbol::getLifetimeEndLocation() const {
        return m_lifetimeEnd;
    }
    
    void Symbol::setLifetimeEndLocation(const SourceLocation& loc) {
        m_lifetimeEnd = loc;
        m_hasLifetimeLoc = true;
    }
};