#pragma once
#include <compile/types.h>
#include <utils/String.h>

namespace compile {
    class Module;
    class Context;

    class IModuleResolver {
        public:
            IModuleResolver();
            virtual ~IModuleResolver();

            virtual Module* resolve(const String& moduleId, Context* ctx) = 0;
    };
};