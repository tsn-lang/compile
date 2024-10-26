#include <compile/interfaces/IModuleSource.h>

namespace compile {
    IModuleSource::IModuleSource(const char *contents, utils::u32 resourceId) : m_resource(contents, resourceId) {
    }

    IModuleSource::~IModuleSource() {
    }
    
    const tokenize::Resource& IModuleSource::getResource() const {
        return m_resource;
    }
};