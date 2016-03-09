#include "catch.hpp"
#include "value.h"

#include <iostream>
#include <sstream>

using namespace Genrile;

TEST_CASE("parse integer", "[Integer]") {
    auto test_int = Integer();

    {
        std::istringstream ss("0");
        test_int.read_json_from_stream(ss);
        REQUIRE(test_int == Integer(0));
    }

    {
        std::istringstream ss("5");
        test_int.read_json_from_stream(ss);
        REQUIRE(test_int == Integer(5));
    }

    {
        std::istringstream ss("-1");
        test_int.read_json_from_stream(ss);
        REQUIRE(test_int == Integer(-1));
    }

    {
        std::istringstream ss("12345678");
        test_int.read_json_from_stream(ss);
        REQUIRE(test_int == Integer(12345678));
    }

    {
        std::istringstream ss("-12345678");
        test_int.read_json_from_stream(ss);
        REQUIRE(test_int == Integer(-12345678));
    }

    {
        std::istringstream ss("0x");
        test_int.read_json_from_stream(ss);
        REQUIRE(test_int == Integer(0));
    }

    {
        std::istringstream ss("5x");
        test_int.read_json_from_stream(ss);
        REQUIRE(test_int == Integer(5));
    }

    {
        std::istringstream ss("-1x");
        test_int.read_json_from_stream(ss);
        REQUIRE(test_int == Integer(-1));
    }

    {
        std::istringstream ss("12345678x");
        test_int.read_json_from_stream(ss);
        REQUIRE(test_int == Integer(12345678));
    }

    {
        std::istringstream ss("-12345678x");
        test_int.read_json_from_stream(ss);
        REQUIRE(test_int == Integer(-12345678));
    }
}

TEST_CASE("parse boolean", "[Boolean]") {
    auto test_bool = Boolean();

    {
        std::istringstream ss("true");
        test_bool.read_json_from_stream(ss);
        REQUIRE(test_bool == Boolean(true));
    }

    {
        std::istringstream ss("false");
        test_bool.read_json_from_stream(ss);
        REQUIRE(test_bool == Boolean(false));
    }

    {
        std::istringstream ss("truethy");
        test_bool.read_json_from_stream(ss);
        REQUIRE(test_bool == Boolean(true));
    }

    {
        std::istringstream ss("falsey");
        test_bool.read_json_from_stream(ss);
        REQUIRE(test_bool == Boolean(false));
    }
}

TEST_CASE("parse string", "[String]") {
    auto test_string = String();

    {
        std::istringstream ss("\"\"");
        test_string.read_json_from_stream(ss);
        REQUIRE(test_string == String(""));
    }

    {
        std::istringstream ss("\"false\"");
        test_string.read_json_from_stream(ss);
        REQUIRE(test_string == String("false"));
    }

    {
        std::istringstream ss("\"true\"");
        test_string.read_json_from_stream(ss);
        REQUIRE(test_string == String("true"));
    }
}
