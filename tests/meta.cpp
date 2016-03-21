#include "catch.hpp"
#include "meta.h"

#include <iostream>
#include <sstream>

using namespace Genrile;

/*
using namespace Genrile::Meta;

TEST_CASE("compile time integer", "[Integer]") {
    static_assert(parse_integer("0").first == 0, "parse failure");
    static_assert(parse_integer("-0").first == 0, "parse failure");
    static_assert(parse_integer("1234").first == 1234, "parse failure");
    static_assert(parse_integer("-1234").first == -1234, "parse failure");
}

#if 0
TEST_CASE("compile time real", "[Real]") {
    static_assert(
        parse_real(ParseState("-9876.543210")).first.value() == -9876.543210,
        "parse failure");
    static_assert(parse_real(ParseState("0.123456789e-12")).first.value() ==
                      0.123456789e-12,
                  "parse failure");
    static_assert(parse_real(ParseState("1.234567890E+34")).first.value() ==
                      1.234567890E+34,
                  "parse failure");
    static_assert(parse_real(ParseState("23456789012E66")).first.value() ==
                      23456789012E66,
                  "parse failure");
}
#endif

TEST_CASE("meta string", "[MetaString]") {
    static_assert(MetaString("").size() == 0, "size failure");
    static_assert(MetaString("\"").size() == 1, "size failure");
    static_assert(MetaString("\"\"").size() == 2, "size failure");
}

TEST_CASE("parse state", "[ParseState]") {
    static_assert(ParseState("").size() == 1, "size failure");
    static_assert(ParseState("\"").size() == 2, "size failure");
    static_assert(ParseState("\"\"").size() == 3, "size failure");

    static_assert(ParseState("").match("").first, "parse failure");
    static_assert(ParseState("a").match("a").first, "parse failure");
    static_assert(ParseState("hello world").match("hello world").first,
                  "parse failure");
    static_assert(ParseState("\"\"").match("\"").first, "parse failure");

    static_assert(ParseState("\"\"").match("\"").second.off() == 1,
                  "index failure");
}

TEST_CASE("linked string", "[LinkedString]") {
}

TEST_CASE("compile time boolean", "[Boolean]") {
    static_assert(parse_boolean("true").first == true, "parse failure");
    static_assert(parse_boolean("false").first == false, "parse failure");
}

TEST_CASE("compile time null", "[Null]") {
    static_assert(parse_null("null").first == Null(), "parse failure");
}

TEST_CASE("compile time string", "[String]") {
    //  basic strings
    static_assert(ParseState(parse_string(R"("")").first).match("").first,
                  "parse failure");
    static_assert(ParseState(parse_string(R"("hello world")").first)
                      .match("hello world")
                      .first,
                  "parse failure");
    static_assert(
        ParseState(parse_string(R"("abcdefg")").first).match("abcdefg").first,
        "parse failure");

    //  escaped characters
    static_assert(
        ParseState(parse_string(R"("\"")").first).match("\"").first,
        "parse failure");
}

TEST_CASE("compile time array", "[Array]") {
    static_assert(parse_array("[]").first == std::array<int, 0>(),
                  "parse error");
    static_assert(
        parse_array("[1, 2, 3, 4]").first == std::array<int, 4>{1, 2, 3, 4},
        "parse error");
}
*/
