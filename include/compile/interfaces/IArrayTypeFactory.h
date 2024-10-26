#pragma once
#include <compile/types.h>
#include <unordered_map>

namespace bind {
    class DataType;
}

namespace compile {
    class IArrayTypeFactory {
        public:
            virtual ~IArrayTypeFactory();

            bind::DataType* getArrayType(bind::DataType* elementType);
        
        protected:
            virtual bind::DataType* generateArrayType(bind::DataType* elementType) = 0;

        private:
            std::unordered_map<bind::symbol_id, bind::DataType*> m_cachedArrayTypes;
    };
};