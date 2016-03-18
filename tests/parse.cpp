#include "catch.hpp"
#include "parse_helpers.h"
#include "value.h"

#include <iostream>
#include <sstream>

using namespace Genrile;

template <typename T>
void int_tests() {
    auto test_read = [](auto str) {
        std::istringstream ss(str);
        return T(ss);
    };

    REQUIRE(test_read("0") == Integer(0));
    REQUIRE(test_read("5") == Integer(5));
    REQUIRE(test_read("-1") == Integer(-1));
    REQUIRE(test_read("12345678") == Integer(12345678));
    REQUIRE(test_read("-12345678") == Integer(-12345678));
    REQUIRE(test_read("0x") == Integer(0));
    REQUIRE(test_read("5x") == Integer(5));
    REQUIRE(test_read("-1x") == Integer(-1));
    REQUIRE(test_read("12345678x") == Integer(12345678));
    REQUIRE(test_read("-12345678x") == Integer(-12345678));
}

TEST_CASE("parse integer", "[Integer]") {
    int_tests<Integer>();
}

template <typename T>
void real_tests() {
    auto test_read = [](auto str) {
        std::istringstream ss(str);
        return T(ss);
    };

    REQUIRE(test_read("0.0") == Real(0));
    REQUIRE(test_read("-9876.543210") == Real(-9876.543210));
    REQUIRE(test_read("0.123456789e-12") == Real(0.123456789e-12));
    REQUIRE(test_read("1.234567890E+34") == Real(1.234567890E+34));
    REQUIRE(test_read("23456789012E66") == Real(23456789012E66));
}

TEST_CASE("parse real", "[Real]") {
    real_tests<Real>();
}

template <typename T>
void bool_tests() {
    auto test_read = [](auto str) {
        std::istringstream ss(str);
        return T(ss);
    };

    REQUIRE(test_read("true") == Boolean(true));
    REQUIRE(test_read("false") == Boolean(false));
    REQUIRE(test_read("truethy") == Boolean(true));
    REQUIRE(test_read("falsey") == Boolean(false));
}

TEST_CASE("parse boolean", "[Boolean]") {
    bool_tests<Boolean>();
}

template <typename T>
void string_tests() {
    auto test_read = [](auto str) {
        std::istringstream ss(str);
        return T(ss);
    };

    REQUIRE(test_read("\"\"") == String(""));
    REQUIRE(test_read("\"false\"") == String("false"));
    REQUIRE(test_read("\"true\"") == String("true"));
}

TEST_CASE("parse string", "[String]") {
    string_tests<String>();
}

template <typename T>
void array_tests() {
    auto test_read = [](auto str) {
        std::istringstream ss(str);
        return T(ss);
    };

    REQUIRE(test_read("[]") == Array());
    REQUIRE(test_read("[[], []]") == Array({Array(), Array()}));
    REQUIRE(test_read("[   [ ]     ,  [    ]  ]  ") ==
            Array({Array(), Array()}));
    REQUIRE(test_read("[1]") == Array({1}));
    REQUIRE(test_read("[1, 2, 3]") == Array({1, 2, 3}));
    REQUIRE(test_read("[1, \"hello\", 3]") == Array({1, "hello", 3}));
    REQUIRE(test_read("[1, \"hello\", 3, [4, 5, 6]]") ==
            Array({1, "hello", 3, Array({4, 5, 6})}));
    REQUIRE(test_read("[1, \"hello\", 3, [4, [], [true, false]]]") ==
            Array({1, "hello", 3, Array({4, Array(), Array({true, false})})}));
}

TEST_CASE("parse array", "[Array]") {
    array_tests<Array>();
}

template <typename T>
void object_tests() {
    auto test_read = [](auto str) {
        std::istringstream ss(str);
        return T(ss);
    };

    REQUIRE(test_read("{}") == Object());
    REQUIRE(test_read("{     \"str\"     :   \"str\"    }   ") ==
            Object({{"str", "str"}}));
    REQUIRE(test_read("{     \"an object\"     :   {}    }   ") ==
            Object({{"an object", Object()}}));
    REQUIRE(test_read("{\"the array\":  [1, 2, 3, 4], \"another thing\": 5}") ==
            Object({{"the array", Array({1, 2, 3, 4})}, {"another thing", 5}}));
    REQUIRE(
        test_read(
            "{     \"an object containing an array\"     :   {\"the array\":  "
            "[1, 2, 3, 4], \"another thing\": 5}    }   ") ==
        Object({{"an object containing an array",
                 Object({{"the array", Array({1, 2, 3, 4})},
                         {"another thing", 5}})}}));
}

TEST_CASE("parse object", "[Object]") {
    object_tests<Object>();
}

TEST_CASE("parse variadic", "[Variadic]") {
    int_tests<Variadic>();
    bool_tests<Variadic>();
    string_tests<Variadic>();
    array_tests<Variadic>();
    object_tests<Variadic>();
}

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

    std::istringstream ss(test_string);
    auto v = Variadic(ss);
    v.write_json_to_stream(std::cout);
}
