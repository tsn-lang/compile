#pragma once
#include <compile/types.h>

namespace bind {
    class DataType;
};

namespace parse {
    class Node;
};

namespace compile {
    class Context;

    class TypeArgument {
        public:
            TypeArgument(Context* ctx, bind::DataType* type, parse::Node* node);

            void logDebug(const char* msg, ...);
            void logInfo(const char* msg, ...);
            void logWarn(const char* msg, ...);
            void logError(const char* msg, ...);

            bind::DataType* getType() const;
            parse::Node* getNode() const;
        
        protected:
            Context* m_ctx;
            bind::DataType* m_type;
            parse::Node* m_argNode;
    };
};