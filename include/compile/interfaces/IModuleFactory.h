#pragma once
#include <compile/types.h>

namespace compile {
    class Context;
    class Module;

    class IModuleFactory {
        public:
            IModuleFactory();
            virtual ~IModuleFactory();

            virtual Module* createModule(Context* ctx) = 0;
            virtual void destroyModule(Module* mod, Context* ctx) = 0;
    };
};