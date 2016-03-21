#include "catch.hpp"
#include "meta_parser.h"

#include "boost/mpl/vector.hpp"
#include "boost/mpl/equal.hpp"

#include <iostream>

using boost::mpl::equal;
using boost::mpl::vector;
using boost::mpl::map;
using boost::mpl::pair;
using boost::mpl::integral_c;

using mpllibs::metaparse::build_parser;
using mpllibs::metaparse::is_error;

using genrile::metaparse::integer_parser;
using genrile::metaparse::string_parser;
using genrile::metaparse::array_comma_element;
using genrile::metaparse::array_elements_parser;
using genrile::metaparse::array_parser;
using genrile::metaparse::wrap_into_vector;
using genrile::metaparse::object_pair_parser;
using genrile::metaparse::object_parser;
using genrile::metaparse::wrap_into_map;
using genrile::metaparse::null_parser;
using genrile::metaparse::boolean_parser;
using genrile::metaparse::value_parser;
using genrile::metaparse::null;

template <typename T>
class PrintType;

template <typename PARSER, typename STR>
using run_parser = typename build_parser<PARSER>::template apply<STR>::type;

template <typename P>
void integer_tests() {
    {
        using parser_type = run_parser<P, MPLLIBS_STRING("0")>;
        static_assert(!is_error<parser_type>::type::value, "parse failure");
        static_assert(parser_type::value == 0, "parse failure");
    }

    {
        using parser_type = run_parser<P, MPLLIBS_STRING("1")>;
        static_assert(!is_error<parser_type>::type::value, "parse failure");
        static_assert(parser_type::value == 1, "parse failure");
    }

    /*
    {
        using parser_type = build_parser<integer_parser>::apply<MPLLIBS_STRING(
            "-2")>::type;
        static_assert(parser_type::value == -2, "parse failure");
    }
    */
}

TEST_CASE("integer", "[Integer]") {
    integer_tests<integer_parser>();
}

template <typename P>
void string_tests() {
    {
        using parser_type = run_parser<P, MPLLIBS_STRING("\"\"")>;
        static_assert(!is_error<parser_type>::type::value, "parse failure");
        static_assert(equal<parser_type, MPLLIBS_STRING("")>::value,
                      "parse failure");
    }
    {
        using parser_type = run_parser<P, MPLLIBS_STRING("\"hello world\"")>;
        static_assert(!is_error<parser_type>::type::value, "parse failure");
        static_assert(equal<parser_type, MPLLIBS_STRING("hello world")>::value,
                      "parse failure");
    }
}

TEST_CASE("string", "[String]") {
    string_tests<string_parser>();
}

TEST_CASE("array parser internals", "[Array]") {
    {
        using parser_type =
            run_parser<array_comma_element, MPLLIBS_STRING(",   1")>;
        static_assert(!is_error<parser_type>::type::value, "parse failure");
        static_assert(parser_type() == 1, "parse failure");
    }
    {
        using parser_type =
            run_parser<array_comma_element, MPLLIBS_STRING(",\"hello world\"")>;
        static_assert(!is_error<parser_type>::type::value, "parse failure");
        static_assert(equal<parser_type, MPLLIBS_STRING("hello world")>::value,
                      "parse failure");
    }

    {
        using parser_type =
            run_parser<wrap_into_vector<value_parser>, MPLLIBS_STRING("1")>;
        static_assert(!is_error<parser_type>::type::value, "parse failure");
        static_assert(equal<parser_type, vector<integral_c<int, 1>>>::value,
                      "parse failure");
    }

    {
        using parser_type = run_parser<wrap_into_vector<value_parser>,
                                       MPLLIBS_STRING("\"hello world\"")>;
        static_assert(!is_error<parser_type>::type::value, "parse failure");
        static_assert(
            equal<parser_type, vector<MPLLIBS_STRING("hello world")>>::value,
            "parse failure");
    }

    {
        using parser_type =
            run_parser<array_elements_parser, MPLLIBS_STRING("1")>;
        static_assert(!is_error<parser_type>::type::value, "parse failure");
        static_assert(equal<parser_type, vector<integral_c<int, 1>>>::value,
                      "parse failure");
    }

    {
        using parser_type =
            run_parser<array_elements_parser, MPLLIBS_STRING("1, 2")>;
        static_assert(!is_error<parser_type>::type::value, "parse failure");
        static_assert(
            equal<parser_type,
                  vector<integral_c<int, 1>, integral_c<int, 2>>>::value,
            "parse failure");
    }
}

template <typename P>
void array_tests() {
    {
        using parser_type = run_parser<P, MPLLIBS_STRING("[]")>;
        static_assert(!is_error<parser_type>::type::value, "parse failure");
        static_assert(equal<parser_type, vector<>>::value, "parse failure");
    }
    {
        using parser_type = run_parser<P, MPLLIBS_STRING("[1]")>;
        static_assert(!is_error<parser_type>::type::value, "parse failure");
        static_assert(equal<parser_type, vector<integral_c<int, 1>>>::value,
                      "parse failure");
    }
    {
        using parser_type = run_parser<P, MPLLIBS_STRING("[1, 2, 3]")>;
        static_assert(!is_error<parser_type>::type::value, "parse failure");
        static_assert(equal<parser_type,
                            vector<integral_c<int, 1>,
                                   integral_c<int, 2>,
                                   integral_c<int, 3>>>::value,
                      "parse failure");
    }
    {
        using parser_type =
            run_parser<P,
                       MPLLIBS_STRING("[[1, 2, \"true\", false, null], "
                                      "true, [], 4, 5, \"hello world\"]")>;
        static_assert(!is_error<parser_type>::type::value, "parse failure");
    }
}

TEST_CASE("array", "[Array]") {
    array_tests<array_parser>();
}

TEST_CASE("object parser internals", "[Object]") {
    {
        using parser_type = run_parser<object_pair_parser,
                                       MPLLIBS_STRING("\"key\" : \"value\"")>;
        static_assert(!is_error<parser_type>::type::value, "parse failure");
        static_assert(
            std::is_same<
                parser_type,
                pair<MPLLIBS_STRING("key"), MPLLIBS_STRING("value")>>::value,
            "parse failure");
    }

    {
        using parser_type =
            run_parser<object_pair_parser, MPLLIBS_STRING("\"key\":[1, 2, 3]")>;
        static_assert(!is_error<parser_type>::type::value, "parse failure");
    }
}

template <typename P>
void object_tests() {
    {
        using parser_type = run_parser<P, MPLLIBS_STRING("{}")>;
        static_assert(!is_error<parser_type>::type::value, "parse failure");
        static_assert(equal<parser_type, map<>>::value, "parse failure");
    }

    {
        using parser_type = run_parser<P, MPLLIBS_STRING("{\"str\":1}")>;
        static_assert(!is_error<parser_type>::type::value, "parse failure");
        static_assert(
            equal<parser_type,
                  map<pair<MPLLIBS_STRING("str"), integral_c<int, 1>>>>::value,
            "parse failure");
    }

    {
        using parser_type =
            run_parser<P, MPLLIBS_STRING("{\"str\":1, \"str 2\":2}")>;
        static_assert(!is_error<parser_type>::type::value, "parse failure");
    }

    {
        using parser_type =
            run_parser<P,
                       MPLLIBS_STRING("{\"str\":1, \"str 2\":2, \"an array\"  "
                                      ": [1, 2, [], 4] , \"a null\" : null }")>;
        static_assert(!is_error<parser_type>::type::value, "parse failure");
    }
}

TEST_CASE("object", "[Object]") {
    object_tests<object_parser>();
}

template <typename P>
void null_tests() {
    {
        using parser_type = run_parser<P, MPLLIBS_STRING("null")>;
        static_assert(!is_error<parser_type>::type::value, "parse failure");
        static_assert(std::is_same<parser_type, null>::value, "parse failure");
    }
}

TEST_CASE("null", "[Null]") {
    null_tests<null_parser>();
}

template <typename P>
void boolean_tests() {
    {
        using parser_type = run_parser<P, MPLLIBS_STRING("true")>;
        static_assert(!is_error<parser_type>::type::value, "parse failure");
        static_assert(parser_type(), "parse failure");
    }

    {
        using parser_type = run_parser<P, MPLLIBS_STRING("false")>;
        static_assert(!is_error<parser_type>::type::value, "parse failure");
        static_assert(!parser_type(), "parse failure");
    }
}

TEST_CASE("boolean", "[Boolean]") {
    boolean_tests<boolean_parser>();
}

TEST_CASE("value", "[Value]") {
    null_tests<value_parser>();
    boolean_tests<value_parser>();
    string_tests<value_parser>();
    array_tests<value_parser>();
    object_tests<value_parser>();
    integer_tests<value_parser>();
}
