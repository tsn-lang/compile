#include <compile/TypeArgument.h>
#include <compile/Context.h>
#include <stdarg.h>
#include <stdio.h>

namespace compile {
    TypeArgument::TypeArgument(Context* ctx, bind::DataType* type, parse::Node* node) : m_ctx(ctx), m_type(type), m_argNode(node) {
    }

    void TypeArgument::logDebug(const char* msg, ...) {
        char buf[1024] = { 0 };

        va_list l;
        va_start(l, msg);
        vsnprintf(buf, 1024, msg, l);
        va_end(l);

        m_ctx->enterNode(m_argNode);
        m_ctx->logDebug(buf);
        m_ctx->exitNode();
    }

    void TypeArgument::logInfo(const char* msg, ...) {
        char buf[1024] = { 0 };

        va_list l;
        va_start(l, msg);
        vsnprintf(buf, 1024, msg, l);
        va_end(l);

        m_ctx->enterNode(m_argNode);
        m_ctx->logInfo(buf);
        m_ctx->exitNode();
    }

    void TypeArgument::logWarn(const char* msg, ...) {
        char buf[1024] = { 0 };

        va_list l;
        va_start(l, msg);
        vsnprintf(buf, 1024, msg, l);
        va_end(l);

        m_ctx->enterNode(m_argNode);
        m_ctx->logWarn(buf);
        m_ctx->exitNode();
    }

    void TypeArgument::logError(const char* msg, ...) {
        char buf[1024] = { 0 };

        va_list l;
        va_start(l, msg);
        vsnprintf(buf, 1024, msg, l);
        va_end(l);

        m_ctx->enterNode(m_argNode);
        m_ctx->logError(buf);
        m_ctx->exitNode();
    }

    bind::DataType* TypeArgument::getType() const {
        return m_type;
    }

    parse::Node* TypeArgument::getNode() const {
        return m_argNode;
    }
};