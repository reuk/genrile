#include "catch.hpp"
#include "value.h"
#include "parse_helpers.h"

#include <iostream>
#include <sstream>

using namespace Genrile;

template <typename T>
void int_tests() {
    auto test_int = T();

    auto test_read = [&test_int](auto str) {
        std::istringstream ss(str);
        test_int.read_json_from_stream(ss);
    };

    test_read("0");
    REQUIRE(test_int == Integer(0));

    test_read("5");
    REQUIRE(test_int == Integer(5));

    test_read("-1");
    REQUIRE(test_int == Integer(-1));

    test_read("12345678");
    REQUIRE(test_int == Integer(12345678));

    test_read("-12345678");
    REQUIRE(test_int == Integer(-12345678));

    test_read("0x");
    REQUIRE(test_int == Integer(0));

    test_read("5x");
    REQUIRE(test_int == Integer(5));

    test_read("-1x");
    REQUIRE(test_int == Integer(-1));

    test_read("12345678x");
    REQUIRE(test_int == Integer(12345678));

    test_read("-12345678x");
    REQUIRE(test_int == Integer(-12345678));
}

TEST_CASE("parse integer", "[Integer]") {
    int_tests<Integer>();
}

template <typename T>
void bool_tests() {
    auto test_bool = T();

    auto test_read = [&test_bool](auto str) {
        std::istringstream ss(str);
        test_bool.read_json_from_stream(ss);
    };

    test_read("true");
    REQUIRE(test_bool == Boolean(true));

    test_read("false");
    REQUIRE(test_bool == Boolean(false));

    test_read("truethy");
    REQUIRE(test_bool == Boolean(true));

    test_read("falsey");
    REQUIRE(test_bool == Boolean(false));
}

TEST_CASE("parse boolean", "[Boolean]") {
    bool_tests<Boolean>();
}

template <typename T>
void string_tests() {
    auto test_string = T();

    auto test_read = [&test_string](auto str) {
        std::istringstream ss(str);
        test_string.read_json_from_stream(ss);
    };

    test_read("\"\"");
    REQUIRE(test_string == String(""));

    test_read("\"false\"");
    REQUIRE(test_string == String("false"));

    test_read("\"true\"");
    REQUIRE(test_string == String("true"));
}

TEST_CASE("parse string", "[String]") {
    string_tests<String>();
}

template <typename T>
void array_tests() {
    auto test_array = T();

    auto test_read = [&test_array](auto str) {
        std::istringstream ss(str);
        test_array.read_json_from_stream(ss);
    };

    test_read("[]");
    REQUIRE(test_array == Array());

    test_read("[[], []]");
    REQUIRE(test_array == Array({Array(), Array()}));

    test_read("[1]");
    REQUIRE(test_array == Array({1}));

    test_read("[1, 2, 3]");
    REQUIRE(test_array == Array({1, 2, 3}));

    test_read("[1, \"hello\", 3]");
    REQUIRE(test_array == Array({1, "hello", 3}));

    test_read("[1, \"hello\", 3, [4, 5, 6]]");
    REQUIRE(test_array == Array({1, "hello", 3, Array({4, 5, 6})}));

    test_read("[1, \"hello\", 3, [4, [], [true, false]]]");
    REQUIRE(test_array ==
            Array({1, "hello", 3, Array({4, Array(), Array({true, false})})}));
}

TEST_CASE("parse array", "[Array]") {
    array_tests<Array>();
}

template <typename T>
void object_tests() {
    auto test_object = T();

    auto test_read = [&test_object](auto str) {
        std::istringstream ss(str);
        test_object.read_json_from_stream();
    };

    test_read("{}");
    REQUIRE(test_object == Object());

    test_read("    {     \"str\"     :   \"str\"    }   ");
    REQUIRE(test_object == Object({{"str", "str"}}));

    test_read("    {     \"an object\"     :   {}    }   ");
    REQUIRE(test_object == Object({{"str", Object()}}));

    test_read(
        "    {     \"an object containing an array\"     :   {\"the array\":  "
        "[1, 2, 3, 4], \"another thing\": 5}    }   ");
    REQUIRE(test_object == Object({{"an object containing an array",
                                    Object({{"the array", Array({1, 2, 3, 4})},
                                            {"another thing", 5}})}}));
}

TEST_CASE("parse variadic", "[Variadic]") {
    int_tests<Variadic>();
    bool_tests<Variadic>();
    string_tests<Variadic>();
    array_tests<Variadic>();
}

#if 0
TEST_CASE("big string", "[Variadic]") {
    auto test_string = R"(
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
)";

    auto v = Variadic();
    std::istringstream ss(test_string);
    v.read_json_from_stream(ss);
}
#endif
