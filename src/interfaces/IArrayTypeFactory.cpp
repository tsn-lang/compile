#include <compile/interfaces/IArrayTypeFactory.h>
#include <bind/DataType.h>

namespace compile {
    IArrayTypeFactory::~IArrayTypeFactory() {
    }

    bind::DataType* IArrayTypeFactory::getArrayType(bind::DataType* elementType) {
        auto it = m_cachedArrayTypes.find(elementType->getSymbolId());
        if (it != m_cachedArrayTypes.end()) return it->second;

        bind::DataType* output = generateArrayType(elementType);
        if (!output) return nullptr;

        m_cachedArrayTypes.insert(std::pair(elementType->getSymbolId(), output));
        return output;
    }
};