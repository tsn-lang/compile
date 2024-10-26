#pragma once
#include <compile/types.h>

namespace codegen {
    class IBackend;
};

namespace compile {
    class IModuleResolver;
    class IModuleFactory;

    class Compiler {
        public:
            Compiler(
                codegen::IBackend* backend,
                IModuleResolver* moduleResolver,
                IModuleFactory* moduleFactory
            );
            ~Compiler();

            codegen::IBackend* getBackend() const;
            IModuleResolver* getModuleResolver() const;
            IModuleFactory* getModuleFactory() const;
        
        private:
            codegen::IBackend* m_backend;
            IModuleResolver* m_moduleResolver;
            IModuleFactory* m_moduleFactory;
    };
};