#define MPLLIBS_LIMIT_STRING_SIZE 256

#include "catch.hpp"

#include "meta_parser.h"

#include <mpllibs/metaparse/build_parser.hpp>
#include <mpllibs/metaparse/is_error.hpp>

template <typename T>
struct PrintType;

using genrile::metaparse::json_parser;

using mpllibs::metaparse::is_error;

template <typename PARSER, typename STR>
using run_parser = typename mpllibs::metaparse::build_parser<
    PARSER>::template apply<STR>::type;

template <typename STR>
using run_json_parser = typename run_parser<json_parser, STR>::type;

TEST_CASE("slightly evil", "[Slightly Evil]") {
    using parsed_type = run_json_parser<MPLLIBS_STRING(R"([
    "JSON Test Pattern pass1",
    "rosebud",
    {"a double": 1.234e45},
    {"object with array of some types":[true, false, null, [], {}]}
    ])")>;
    static_assert(!is_error<parsed_type>::type::value, "parse failure");
    PrintType<parsed_type> p;
}

#if 0
TEST_CASE("evil", "[Pure Evil]") {
    using parser_type =
        run_parser<json_parser, MPLLIBS_STRING(R"(
[
    "JSON Test Pattern pass1",
    {"object with 1 member":["array with 1 element"]},
    {},
    [],
    -42,
    true,
    false,
    null,
    {
        "integer": 1234567890,
        "real": -9876.543210,
        "e": 0.123456789e-12,
        "E": 1.234567890E+34,
        "":  23456789012E66,
        "zero": 0,
        "one": 1,
        "space": " ",
        "quote": "\"",
        "backslash": "\\",
        "controls": "\b\f\n\r\t",
        "slash": "/ & \/",
        "alpha": "abcdefghijklmnopqrstuvwyz",
        "ALPHA": "ABCDEFGHIJKLMNOPQRSTUVWYZ",
        "digit": "0123456789",
        "0123456789": "digit",
        "special": "`1~!@#$%^&*()_+-={':[,]}|;.</>?",
        "hex": "\u0123\u4567\u89AB\uCDEF\uabcd\uef4A",
        "true": true,
        "false": false,
        "null": null,
        "array":[  ],
        "object":{  },
        "address": "50 St. James Street",
        "url": "http://www.JSON.org/",
        "comment": "// /* <!-- --",
        "# -- --> */": " ",
        " s p a c e d " :[1,2 , 3

,

4 , 5        ,          6           ,7        ],"compact":[1,2,3,4,5,6,7],
        "jsontext": "{\"object with 1 member\":[\"array with 1 element\"]}",
        "quotes": "&#34; \u0022 %22 0x22 034 &#x22;",
        "\/\\\"\uCAFE\uBABE\uAB98\uFCDE\ubcda\uef4A\b\f\n\r\t`1~!@#$%^&*()_+-=[]{}|;:',./<>?"
: "A key can be any string"
    },
    0.5 ,98.6
,
99.44
,

1066,
1e1,
0.1e1,
1e-1,
1e00,2e+00,2e-00
,"rosebud"]
)")>;

    static_assert(!is_error<parser_type>::type::value, "parse failure");
}
#endif
