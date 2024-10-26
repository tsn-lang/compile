#include <catch2/catch_test_macros.hpp>
#include <compile/Context.h>
#include <compile/Compiler.h>
#include <compile/interfaces/IModuleResolver.h>
#include <compile/interfaces/IModuleFactory.h>
#include <compile/interfaces/IModuleSource.h>
#include <codegen/TestBackend.h>
#include <utils/String.h>

using namespace compile;

namespace Catch {
    template<>
    struct StringMaker<utils::String> {
        static std::string convert( utils::String const& value ) {
            return utils::String::Format("\"%s\"", value.c_str());
        }
    };
};

class ModuleSource : public IModuleSource {
    public:
        ModuleSource(const char* moduleId, const char* contents);
        ~ModuleSource();
        
        String moduleId;
        Module* output;

    protected:
        static u32 nextResourceId;
};

class TestContext : public IModuleResolver, public IModuleFactory {
    public:
        TestContext(const char* mainSource);
        ~TestContext();

        bool compile();

        ModuleSource* addSource(const char* moduleId, const char* src);

        virtual Module* resolve(const String& moduleId, Context* ctx);
        virtual Module* createModule(Context* ctx);
        virtual void destroyModule(Module* mod, Context* ctx);

        Array<ModuleSource> sources;
        codegen::TestBackend backend;
        Compiler compiler;
        Context ctx;
};