#pragma once
#include <compile/types.h>
#include <tokenize/Resource.h>

namespace compile {
    class IModuleSource {
        public:
            IModuleSource(const char *contents, utils::u32 resourceId);
            virtual ~IModuleSource();

            const tokenize::Resource& getResource() const;

        protected:
            tokenize::Resource m_resource;
    };
};