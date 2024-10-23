#include "Common.h"

void setupTest() {
}

TestContext::TestContext(const char* source) : ctx(nullptr), tokenSet(nullptr), src(nullptr), tokenizedSrc(nullptr) {
    tokenSet = new tokenize::TokenSet();
    parse::setupTokenSet(tokenSet);

    src = new tokenize::Resource(source, 1);
    tokenizedSrc = src->tokenize(tokenSet);
    ctx = new Context(tokenizedSrc);
}

TestContext::~TestContext() {
    delete ctx;
    ctx = nullptr;

    delete tokenizedSrc;
    tokenizedSrc = nullptr;

    delete src;
    src = nullptr;

    delete tokenSet;
    tokenSet = nullptr;
}