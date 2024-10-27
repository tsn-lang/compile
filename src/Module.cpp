#include <compile/Module.h>
#include <bind/Namespace.h>
#include <bind/Registry.h>

namespace compile {
    Module::Module(bind::Namespace* parent, const String& name) {
        m_namespace = new bind::Namespace(parent, name);
    }

    Module::~Module() {
        delete m_namespace;
    }

    bind::Namespace* Module::getNamespace() const {
        return m_namespace;
    }
};