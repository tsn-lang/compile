#include "Common.h"
#include <compile/Module.h>
#include <compile/types/BuiltinArray.h>
#include <bind/bind.hpp>
#include <utils/Array.hpp>

u32 ModuleSource::nextResourceId = 1;
ModuleSource::ModuleSource(const char* _moduleId, const char* src) : IModuleSource(src, ModuleSource::nextResourceId++), moduleId(_moduleId) {
    output = nullptr;
}

ModuleSource::~ModuleSource() {
}



TestContext::TestContext(const char* mainSource) : compiler(&backend, this, this), ctx(&compiler, addSource("main", mainSource)) {
    bind::Registry::Create();
    bind::type<void>("void");
    bind::type<bool>("bool");
    bind::type<i8>("i8");
    bind::type<i16>("i16");
    bind::type<i32>("i32");
    bind::type<i64>("i64");
    bind::type<u8>("u8");
    bind::type<u16>("u16");
    bind::type<u32>("u32");
    bind::type<u64>("u64");
    bind::type<f32>("f32");
    bind::type<f64>("f64");
    bind::type<poison_t>("error-type");
    BuiltinArray::Register();
}

TestContext::~TestContext() {
    ctx.discardOutput();
    bind::Registry::Destroy();
}

bool TestContext::compile() {
    return ctx.compile();
}

ModuleSource* TestContext::addSource(const char* moduleId, const char* source) {
    sources.emplace(moduleId, source);
    return &sources.last();
}

Module* TestContext::resolve(const String& moduleId, Context* ctx) {
    for (u32 i = 0;i < sources.size();i++) {
        if (sources[i].moduleId == moduleId) {
            if (!sources[i].output) {
                Context comp(&compiler, &sources[i]);
                if (!comp.compile()) {
                    ctx->logError("Failed to compile module '%s'", moduleId);
                    return nullptr;
                }

                sources[i].output = comp.getModule();
            }
            
            return sources[i].output;
        }
    }

    ctx->logError("Failed to resolve module '%s'");
    return nullptr;
}

Module* TestContext::createModule(Context* ctx) {
    ModuleSource* src = (ModuleSource*)ctx->getModuleSource();
    return new Module(nullptr, src->moduleId);
}

void TestContext::destroyModule(Module* mod, Context* ctx) {
    delete mod;
}