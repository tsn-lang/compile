#pragma once
#include <compile/types.h>
#include <bind/Namespace.h>
#include <utils/String.h>
#include <utils/Array.h>

namespace compile {
    class Symbol;

    class Module {
        public:
            Module(bind::Namespace* parent, const String& name);
            ~Module();

            bind::Namespace* getNamespace() const;
        
        protected:
            bind::Namespace* m_namespace;
    };
};