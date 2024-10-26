#include "Common.h"
#include <utils/Array.hpp>

TEST_CASE("Playground", "[compile]") {
    TestContext test(
        "function test() : void {\n"
        "    const a = 1;\n"
        "}"
    );
    REQUIRE(test.compile() == true);
}