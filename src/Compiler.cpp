#include <compile/Compiler.h>

namespace compile {
    Compiler::Compiler(
        codegen::IBackend* backend,
        IModuleResolver* moduleResolver,
        IModuleFactory* moduleFactory
    ) : m_backend(backend), m_moduleResolver(moduleResolver), m_moduleFactory(moduleFactory)
    {
    }

    Compiler::~Compiler() {
    }

    codegen::IBackend* Compiler::getBackend() const {
        return m_backend;
    }

    IModuleResolver* Compiler::getModuleResolver() const {
        return m_moduleResolver;
    }

    IModuleFactory* Compiler::getModuleFactory() const {
        return m_moduleFactory;
    }
};