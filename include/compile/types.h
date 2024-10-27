#pragma once
#include <parse/types.h>
#include <codegen/types.h>

namespace tokenize {
    struct SourceLocation;
}

namespace compile {
    using namespace utils;
    using tokenize::SourceLocation;

    struct poison_t {};
    struct array_t {};

    struct tsn_type_userdata {
        struct {
            unsigned indicator_bit : 1;
            unsigned is_host_template : 1;
            unsigned is_script_template : 1;
        } flags;
    };

    struct tsn_property_userdata {
        struct {
            unsigned indicator_bit : 1;
            unsigned is_host_template_method : 1;
            unsigned is_script_template_method : 1;
        } flags;
    };

    struct tsn_func_userdata {
        struct {
            unsigned indicator_bit : 1;
        } flags;
    };
};