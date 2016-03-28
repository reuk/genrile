#define MPLLIBS_LIMIT_STRING_SIZE 128

#include "catch.hpp"

#include "meta_parser.h"
#include "constexpr.h"

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
using genrile::metaparse::real_parser;
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
using genrile::metaparse::Null;
using genrile::metaparse::positive_int_parser;

using genrile::metaparse::constexpr_transform_trait;

template <typename T>
class PrintType;

template <int I>
class PrintInt;

template <typename PARSER, typename STR>
using run_parser = typename build_parser<PARSER>::template apply<STR>::type;

TEST_CASE("scratch", "[Scratch]") {
    {
        using parser_type = run_parser<genrile::metaparse::integer_parser,
                                       MPLLIBS_STRING("-123")>;
        static_assert(!is_error<parser_type>::type::value, "parse failure");
    }

    {
        using parser_type =
            run_parser<genrile::metaparse::frac_parser, MPLLIBS_STRING(".123")>;
        static_assert(!is_error<parser_type>::type::value, "parse failure");
    }

    {
        using parser_type =
            run_parser<genrile::metaparse::exp_parser, MPLLIBS_STRING("E+123")>;
        static_assert(!is_error<parser_type>::type::value, "parse failure");
    }

    {
        using parser_type = run_parser<genrile::metaparse::real_parser,
                                       MPLLIBS_STRING("123.46E+123")>;
        static_assert(!is_error<parser_type>::type::value, "parse failure");
    }

    {
        using parser_type = run_parser<genrile::metaparse::unicode_parser,
                                       MPLLIBS_STRING("2b1c")>;
        static_assert(!is_error<parser_type>::type::value, "parse failure");
    }
}

template <typename P>
void integer_tests() {
    static_assert(genrile::metaparse::digit_to_int('0') == 0, "value failure");
    static_assert(genrile::metaparse::digit_to_int('1') == 1, "value failure");
    static_assert(genrile::metaparse::digit_to_int('2') == 2, "value failure");
    static_assert(genrile::metaparse::digit_to_int('3') == 3, "value failure");
    static_assert(genrile::metaparse::digit_to_int('4') == 4, "value failure");
    static_assert(genrile::metaparse::digit_to_int('5') == 5, "value failure");
    static_assert(genrile::metaparse::digit_to_int('6') == 6, "value failure");
    static_assert(genrile::metaparse::digit_to_int('7') == 7, "value failure");
    static_assert(genrile::metaparse::digit_to_int('8') == 8, "value failure");
    static_assert(genrile::metaparse::digit_to_int('9') == 9, "value failure");

    static_assert(
        constexpr_transform_trait<genrile::metaparse::boxed_integer_string<
                MPLLIBS_STRING("0")>>::run() == 0,
        "conversion failure");
    static_assert(
        constexpr_transform_trait<genrile::metaparse::boxed_integer_string<
                MPLLIBS_STRING("1")>>::run() == 1,
        "conversion failure");
    static_assert(
        constexpr_transform_trait<genrile::metaparse::boxed_integer_string<
                MPLLIBS_STRING("-1")>>::run() == -1,
        "conversion failure");

    static_assert(
        constexpr_transform_trait<genrile::metaparse::boxed_integer_string<
                MPLLIBS_STRING("-12345")>>::run() == -12345,
        "conversion failure");

    static_assert(
        constexpr_transform_trait<genrile::metaparse::boxed_integer_string<
                MPLLIBS_STRING("987")>>::run() == 987,
        "conversion failure");

    {
        using parser_type = run_parser<P, MPLLIBS_STRING("0")>;
        static_assert(!is_error<parser_type>::type::value, "parse failure");
        constexpr auto value = constexpr_transform_trait<parser_type>::run();
        static_assert(value == 0, "parse failure");
    }

    {
        using parser_type = run_parser<P, MPLLIBS_STRING("2")>;
        static_assert(!is_error<parser_type>::type::value, "parse failure");
        constexpr auto value = constexpr_transform_trait<parser_type>::run();
        static_assert(value == 2, "parse failure");
    }

    {
        using parser_type =
            build_parser<integer_parser>::apply<MPLLIBS_STRING("-2")>::type;
        static_assert(!is_error<parser_type>::type::value, "parse failure");
        //        static_assert(parser_type::value == -2, "parse failure");
        constexpr auto value = constexpr_transform_trait<parser_type>::run();
        static_assert(value == -2, "parse failure");
    }

    {
        using parser_type =
            build_parser<integer_parser>::apply<MPLLIBS_STRING("-0")>::type;
        static_assert(!is_error<parser_type>::type::value, "parse failure");
        constexpr auto value = constexpr_transform_trait<parser_type>::run();
        static_assert(value == 0, "parse failure");
    }

    {
        using parser_type = build_parser<integer_parser>::apply<MPLLIBS_STRING(
            "-123456")>::type;
        static_assert(!is_error<parser_type>::type::value, "parse failure");
        constexpr auto value = constexpr_transform_trait<parser_type>::run();
        static_assert(value == -123456, "parse failure");
    }
}

template <typename P>
void digit_tests() {
    {
        using parser_type = run_parser<P, MPLLIBS_STRING("0")>;
        static_assert(!is_error<parser_type>::type::value, "parse failure");
    }

    {
        using parser_type = run_parser<P, MPLLIBS_STRING("2")>;
        static_assert(!is_error<parser_type>::type::value, "parse failure");
    }

    {
        using parser_type = run_parser<P, MPLLIBS_STRING("123456789")>;
        static_assert(!is_error<parser_type>::type::value, "parse failure");
    }
}

TEST_CASE("digit", "[Digit]") {
    digit_tests<integer_parser>();
}

TEST_CASE("integer", "[Integer]") {
    integer_tests<integer_parser>();
}

template <typename P>
void real_tests() {
    static_assert(
        constexpr_transform_trait<genrile::metaparse::boxed_real_string<
                MPLLIBS_STRING("0")>>::run() == 0,
        "conversion failure");

    static_assert(
        constexpr_transform_trait<genrile::metaparse::boxed_real_string<
                MPLLIBS_STRING("0.0")>>::run() == 0.0,
        "conversion failure");

    static_assert(
        constexpr_transform_trait<genrile::metaparse::boxed_real_string<
                MPLLIBS_STRING("0.1")>>::run() == 0.1,
        "conversion failure");

    static_assert(
        constexpr_transform_trait<genrile::metaparse::boxed_real_string<
                MPLLIBS_STRING("123.456")>>::run() == 123.456,
        "conversion failure");

    static_assert(
        constexpr_transform_trait<genrile::metaparse::boxed_real_string<
                MPLLIBS_STRING("-123.456e12")>>::run() == -123.456e12,
        "conversion failure");

    {
        using parser_type = run_parser<P, MPLLIBS_STRING("0.0")>;
        static_assert(!is_error<parser_type>::type::value, "parse failure");
        constexpr auto value = constexpr_transform_trait<parser_type>::run();
        static_assert(value == 0, "parse failure");
    }

    {
        using parser_type = run_parser<P, MPLLIBS_STRING("2.0")>;
        static_assert(!is_error<parser_type>::type::value, "parse failure");
        constexpr auto value = constexpr_transform_trait<parser_type>::run();
        static_assert(value == 2, "parse failure");
    }

    {
        using parser_type =
            build_parser<integer_parser>::apply<MPLLIBS_STRING("-2.0")>::type;
        static_assert(!is_error<parser_type>::type::value, "parse failure");
        //        static_assert(parser_type::value == -2, "parse failure");
        constexpr auto value = constexpr_transform_trait<parser_type>::run();
        static_assert(value == -2, "parse failure");
    }

    {
        using parser_type =
            build_parser<integer_parser>::apply<MPLLIBS_STRING("-0.0")>::type;
        static_assert(!is_error<parser_type>::type::value, "parse failure");
        constexpr auto value = constexpr_transform_trait<parser_type>::run();
        static_assert(value == 0, "parse failure");
    }

    {
        using parser_type = build_parser<integer_parser>::apply<MPLLIBS_STRING(
            "-123456.0")>::type;
        static_assert(!is_error<parser_type>::type::value, "parse failure");
        constexpr auto value = constexpr_transform_trait<parser_type>::run();
        static_assert(value == -123456.0, "parse failure");
    }
}

TEST_CASE("real", "[Real]") {
    real_tests<real_parser>();
}

template <typename P>
void string_tests() {
    {
        using parser_type = run_parser<P, MPLLIBS_STRING(R"("")")>;
        static_assert(!is_error<parser_type>::type::value, "parse failure");
        static_assert(equal<parser_type, MPLLIBS_STRING("")>::value,
                      "parse failure");

        constexpr auto value = constexpr_transform_trait<parser_type>::run();
        static_assert(value.size() == 1, "parse failure");
    }
    {
        using parser_type = run_parser<P, MPLLIBS_STRING(R"("\"")")>;
        static_assert(!is_error<parser_type>::type::value, "parse failure");
        static_assert(equal<parser_type, MPLLIBS_STRING("\"")>::value,
                      "parse failure");

        constexpr auto value = constexpr_transform_trait<parser_type>::run();
        static_assert(value.size() == 2, "parse failure");
    }
    {
        using parser_type = run_parser<P, MPLLIBS_STRING(R"("hello world")")>;
        static_assert(!is_error<parser_type>::type::value, "parse failure");
        static_assert(equal<parser_type, MPLLIBS_STRING("hello world")>::value,
                      "parse failure");

        constexpr auto value = constexpr_transform_trait<parser_type>::run();
        static_assert(value.size() == 12, "parse failure");
        static_assert(value[0] == 'h', "parse failure");
        static_assert(value[10] == 'd', "parse failure");
        static_assert(value[11] == '\0', "parse failure");
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
        //        static_assert(parser_type() == 1, "parse failure");
    }
    {
        using parser_type = run_parser<array_comma_element,
                                       MPLLIBS_STRING(R"(,"hello world")")>;
        static_assert(!is_error<parser_type>::type::value, "parse failure");
        static_assert(equal<parser_type, MPLLIBS_STRING("hello world")>::value,
                      "parse failure");
    }

    {
        using parser_type =
            run_parser<wrap_into_vector<value_parser>, MPLLIBS_STRING("1")>;
        static_assert(!is_error<parser_type>::type::value, "parse failure");
        //         static_assert(equal<parser_type, vector<integral_c<int,
        //         1>>>::value,
        //                       "parse failure");
    }

    {
        using parser_type = run_parser<wrap_into_vector<value_parser>,
                                       MPLLIBS_STRING(R"("hello world")")>;
        static_assert(!is_error<parser_type>::type::value, "parse failure");
        static_assert(
            equal<parser_type, vector<MPLLIBS_STRING("hello world")>>::value,
            "parse failure");
    }

    {
        using parser_type =
            run_parser<array_elements_parser, MPLLIBS_STRING("1")>;
        static_assert(!is_error<parser_type>::type::value, "parse failure");
        //        static_assert(equal<parser_type, vector<integral_c<int,
        //        1>>>::value,
        //                      "parse failure");
    }

    {
        using parser_type =
            run_parser<array_elements_parser, MPLLIBS_STRING("1, 2")>;
        static_assert(!is_error<parser_type>::type::value, "parse failure");
        //        static_assert(
        //            equal<parser_type,
        //                  vector<integral_c<int, 1>, integral_c<int,
        //                  2>>>::value,
        //            "parse failure");
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
        //        static_assert(equal<parser_type, vector<integral_c<int,
        //        1>>>::value,
        //                      "parse failure");
    }
    {
        using parser_type = run_parser<P, MPLLIBS_STRING("[1, 2, 3]")>;
        static_assert(!is_error<parser_type>::type::value, "parse failure");
        //        static_assert(equal<parser_type,
        //                            vector<integral_c<int, 1>,
        //                                   integral_c<int, 2>,
        //                                   integral_c<int, 3>>>::value,
        //                      "parse failure");
    }
    {
        using parser_type = run_parser<
            P,
            MPLLIBS_STRING(
                R"([[1, 2, "true", false, null], true, [], 4, 5, "hello world"])")>;
        static_assert(!is_error<parser_type>::type::value, "parse failure");
    }
}

TEST_CASE("array", "[Array]") {
    array_tests<array_parser>();
}

TEST_CASE("object parser internals", "[Object]") {
    {
        using parser_type = run_parser<object_pair_parser,
                                       MPLLIBS_STRING(R"("key" : "value")")>;
        static_assert(!is_error<parser_type>::type::value, "parse failure");
        static_assert(
            std::is_same<
                parser_type,
                pair<MPLLIBS_STRING("key"), MPLLIBS_STRING("value")>>::value,
            "parse failure");
    }

    {
        using parser_type = run_parser<object_pair_parser,
                                       MPLLIBS_STRING(R"("key":[1, 2, 3])")>;
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
        using parser_type = run_parser<P, MPLLIBS_STRING(R"({"str":1})")>;
        static_assert(!is_error<parser_type>::type::value, "parse failure");
        //        static_assert(
        //            equal<parser_type,
        //                  map<pair<MPLLIBS_STRING("str"), integral_c<int,
        //                  1>>>>::value,
        //            "parse failure");
    }

    {
        using parser_type =
            run_parser<P, MPLLIBS_STRING(R"({"str":1, "str 2":2})")>;
        static_assert(!is_error<parser_type>::type::value, "parse failure");
    }

    {
        using parser_type = run_parser<
            P,
            MPLLIBS_STRING(
                R"({"str":1, "str 2":2, "an array"  : [1, 2, [], 4] , "a null" : null })")>;
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
        static_assert(std::is_same<parser_type, Null>::value, "parse failure");
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
        constexpr auto value = constexpr_transform_trait<parser_type>::run();
        static_assert(value, "parse failure");
    }

    {
        using parser_type = run_parser<P, MPLLIBS_STRING("false")>;
        static_assert(!is_error<parser_type>::type::value, "parse failure");
        static_assert(!parser_type(), "parse failure");
        constexpr auto value = constexpr_transform_trait<parser_type>::run();
        static_assert(!value, "parse failure");
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
    real_tests<value_parser>();
}
