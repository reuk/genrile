#pragma once

#define MPLLIBS_LIMIT_STRING_SIZE 128

#include <mpllibs/metaparse/string.hpp>
#include <mpllibs/metaparse/start.hpp>
#include <mpllibs/metaparse/grammar.hpp>
#include <mpllibs/metaparse/build_parser.hpp>
#include <mpllibs/metaparse/token.hpp>
#include <mpllibs/metaparse/int_.hpp>
#include <mpllibs/metaparse/lit_c.hpp>
#include <mpllibs/metaparse/keyword.hpp>
#include <mpllibs/metaparse/one_of.hpp>
#include <mpllibs/metaparse/middle_of.hpp>
#include <mpllibs/metaparse/any.hpp>
#include <mpllibs/metaparse/one_char_except_c.hpp>
#include <mpllibs/metaparse/is_error.hpp>
#include <mpllibs/metaparse/foldl.hpp>
#include <mpllibs/metaparse/foldlp.hpp>
#include <mpllibs/metaparse/last_of.hpp>
#include <mpllibs/metaparse/transform.hpp>
#include <mpllibs/metaparse/empty.hpp>
#include <mpllibs/metaparse/sequence.hpp>

namespace genrile {
namespace metaparse {

using namespace mpllibs::metaparse;

class null {
public:
    using type = null;
};
class true_ : public std::true_type {
public:
    using type = true_;
};
class false_ : public std::false_type {
public:
    using type = false_;
};

using null_parser = token<keyword<MPLLIBS_STRING("null"), null>::type>::type;

using boolean_parser =
    token<one_of<keyword<MPLLIBS_STRING("true"), true_>::type,
                 keyword<MPLLIBS_STRING("false"), false_>::type>::type>::type;

//  TODO a smarter way of parsing ints that evaluates negative numbers
using integer_parser = token<int_>::type;

//  TODO some way of parsing doubles (maybe using constexpr)

using open_square_token     = token<lit_c<'['>::type>::type;
using closed_square_token   = token<lit_c<']'>::type>::type;
using open_curly_token      = token<lit_c<'{'>::type>::type;
using closed_curly_token    = token<lit_c<'}'>::type>::type;
using comma_token           = token<lit_c<','>::type>::type;
using quote_token           = token<lit_c<'"'>::type>::type;
using colon_token           = token<lit_c<':'>::type>::type;

//  TODO a smarter way of parsing strings that allows for escaping characters
using string_parser = middle_of<
    quote_token,
    foldl<
        one_char_except_c<'"'>::type,
        string<>::type,
        boost::mpl::lambda<
            boost::mpl::push_back<boost::mpl::_1, boost::mpl::_2>>::type>::type,
    quote_token>::type;

struct value_parser;

//  vector helpers
struct empty_vector {
    using type = empty_vector;
    template <typename T>
    struct apply {
        using type = typename boost::mpl::vector0<>::type;
    };
};

struct single_element_vector {
    using type = single_element_vector;
    template <typename T>
    struct apply {
        using type = typename boost::mpl::vector<T>::type;
    };
};

template<typename P>
using wrap_into_vector = typename transform<P, single_element_vector>::type;

//  map helpers
struct empty_map {
    using type = empty_map;
    template <typename T>
    struct apply {
        using type = typename boost::mpl::map0<>::type;
    };
};

struct single_element_map {
    using type = single_element_map;
    template<typename T>
    struct apply {
        using type = typename boost::mpl::map<T>::type;
    };
};

template<typename P>
using wrap_into_map = typename transform<P, single_element_map>::type;

//  object parsers
struct pair_from_object_sequence {
    using type = pair_from_object_sequence;
    template<typename T>
    struct apply {
        using type =
            typename boost::mpl::pair<typename boost::mpl::front<T>::type,
                                      typename boost::mpl::back<T>::type>::type;
    };
};

using object_pair_parser =
    transform<sequence<string_parser, colon_token, value_parser>::type,
              pair_from_object_sequence>::type;

using object_comma_pair = last_of<comma_token, object_pair_parser>::type;

using object_elements_parser =
    foldlp<object_comma_pair,
           wrap_into_map<object_pair_parser>::type,
           boost::mpl::lambda<
               boost::mpl::insert<boost::mpl::_1, boost::mpl::_2>>::type>::type;

using object_parser =
    one_of<transform<sequence<open_curly_token, closed_curly_token>::type,
                     empty_map>::type,
           middle_of<open_curly_token,
                     object_elements_parser,
                     closed_curly_token>::type>::type;

//  array parsers
using array_comma_element = last_of<comma_token, value_parser>::type;

using array_elements_parser = foldlp<
    array_comma_element,
    wrap_into_vector<value_parser>::type,
    boost::mpl::lambda<
        boost::mpl::push_back<boost::mpl::_1, boost::mpl::_2>>::type>::type;

using array_parser =
    one_of<transform<sequence<open_square_token, closed_square_token>::type,
                     empty_vector>::type,
           middle_of<open_square_token,
                     array_elements_parser,
                     closed_square_token>::type>::type;

struct value_parser : public one_of<null_parser,
                                    boolean_parser,
                                    string_parser,
                                    array_parser,
                                    object_parser,
                                    integer_parser>::type {};

using json_parser = one_of<array_parser, object_parser>::type;

}
}
