#pragma once

#include "container_helpers.h"

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
#include <mpllibs/metaparse/foldl1.hpp>
#include <mpllibs/metaparse/foldlp.hpp>
#include <mpllibs/metaparse/last_of.hpp>
#include <mpllibs/metaparse/transform.hpp>
#include <mpllibs/metaparse/empty.hpp>
#include <mpllibs/metaparse/sequence.hpp>
#include <mpllibs/metaparse/digit.hpp>

#include <mpllibs/metamonad/box.hpp>

#include "boost/mpl/copy.hpp"

namespace genrile {
namespace metaparse {

using namespace mpllibs::metaparse;
using namespace mpllibs::metamonad;

class Null {
public:
    using type = Null;
};

using null_parser = token<keyword<MPLLIBS_STRING("null"), Null>::type>::type;

using boolean_parser = token<one_of<
    keyword<MPLLIBS_STRING("true"), std::true_type>::type,
    keyword<MPLLIBS_STRING("false"), std::false_type>::type>::type>::type;

template <typename T>
using literal_keyword = typename keyword<T, T>::type;

using digit_1_9_parser = one_of<lit_c<'1'>::type,
                                lit_c<'2'>::type,
                                lit_c<'3'>::type,
                                lit_c<'4'>::type,
                                lit_c<'5'>::type,
                                lit_c<'6'>::type,
                                lit_c<'7'>::type,
                                lit_c<'8'>::type,
                                lit_c<'9'>::type>::type;

using digit_parser = digit;

using digits_parser = foldl1<
    digit_parser,
    string<>::type,
    boost::mpl::lambda<
        boost::mpl::push_back<boost::mpl::_1, boost::mpl::_2>>::type>::type;

struct join_mpl_strings {
    using type = join_mpl_strings;
    template <typename a, typename b>
    struct apply {
        using type =
            typename boost::mpl::copy<b, boost::mpl::back_inserter<a>>::type;
    };
};

struct concat_mpl_strings {
    using type = concat_mpl_strings;
    template <typename collection>
    struct apply {
        using type = typename boost::mpl::fold<collection,
                                               string<>,
                                               join_mpl_strings>::type;
    };
};

using positive_int_parser =
    token<one_of<transform<sequence<wrap_into_string<digit_1_9_parser>::type,
                                    digits_parser>::type,
                           concat_mpl_strings>::type,
                 wrap_into_string<digit_parser>::type>::type>::type;

using frac_parser = transform<
    sequence<wrap_into_string<lit_c<'.'>::type>::type, digits_parser>::type,
    concat_mpl_strings>::type;

using e_parser = one_of<literal_keyword<MPLLIBS_STRING("e+")>,
                        literal_keyword<MPLLIBS_STRING("e-")>,
                        literal_keyword<MPLLIBS_STRING("e")>,
                        literal_keyword<MPLLIBS_STRING("E+")>,
                        literal_keyword<MPLLIBS_STRING("E-")>,
                        literal_keyword<MPLLIBS_STRING("E")>>::type;

using exp_parser = transform<sequence<e_parser, digits_parser>::type,
                             concat_mpl_strings>::type;

template <typename T>
struct boxed_integer_string : public box<T> {
    using type = boxed_integer_string;
};
template <typename T>
struct boxed_real_string : public box<T> {
    using type = boxed_real_string;
};

struct box_integer_string {
    template <typename T>
    using apply = boxed_integer_string<T>;
};

struct box_real_string {
    template <typename T>
    using apply = boxed_real_string<T>;
};

using i_parser =
    one_of<positive_int_parser,
           transform<sequence<wrap_into_string<lit_c<'-'>::type>::type,
                              positive_int_parser>::type,
                     concat_mpl_strings>::type>::type;

using integer_parser = transform<i_parser, box_integer_string>::type;

using r_parser = one_of<
    transform<sequence<i_parser, frac_parser, exp_parser>::type,
              concat_mpl_strings>::type,
    transform<sequence<i_parser, exp_parser>::type, concat_mpl_strings>::type,
    transform<sequence<i_parser, frac_parser>::type,
              concat_mpl_strings>::type>::type;

using real_parser = transform<r_parser, box_real_string>::type;

//  TODO some way of parsing doubles (maybe using constexpr)

using open_square_token = token<lit_c<'['>::type>::type;
using closed_square_token = token<lit_c<']'>::type>::type;
using open_curly_token = token<lit_c<'{'>::type>::type;
using closed_curly_token = token<lit_c<'}'>::type>::type;
using comma_token = token<lit_c<','>::type>::type;
using quote_token = token<lit_c<'"'>::type>::type;
using colon_token = token<lit_c<':'>::type>::type;

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

//  object parsers
struct pair_from_object_sequence {
    using type = pair_from_object_sequence;
    template <typename T>
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
