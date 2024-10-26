#include <compile/ast/CompileHandler.h>
#include <compile/Context.h>
#include <compile/CompiledFunction.h>


using namespace parse;

namespace compile {
    CompileHandler::CompileHandler(Context* ctx) : m_ctx(ctx) {
    }

    CompileHandler::~CompileHandler() {
    }
};