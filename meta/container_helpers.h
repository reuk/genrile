#pragma once

#include "boost/mpl/vector.hpp"
#include "boost/mpl/map.hpp"

#include <mpllibs/metaparse/string.hpp>
#include <mpllibs/metaparse/transform.hpp>

namespace genrile {
namespace metaparse {

template <typename T>
struct empty_container {
    using type = empty_container;
    template <typename>
    struct apply {
        using type = typename T::type;
    };
};

template <template <typename... _> class T>
struct single_element_container {
    using type = single_element_container;
    template <typename... U>
    struct apply {
        using type = typename T<U...>::type;
    };
};

using empty_vector = empty_container<boost::mpl::vector<>::type>::type;
using empty_map = empty_container<boost::mpl::map<>::type>::type;

using empty_string = empty_container<mpllibs::metaparse::string<>::type>::type;

using single_element_vector =
    single_element_container<boost::mpl::vector>::type;
using single_element_map = single_element_container<boost::mpl::map>::type;

struct single_element_string {
    using type = single_element_string;
    template <typename... U>
    struct apply {
        using type = typename mpllibs::metaparse::string<U::value...>::type;
    };
};

template <typename P>
using wrap_into_vector =
    typename mpllibs::metaparse::transform<P, single_element_vector>::type;
template <typename P>
using wrap_into_map =
    typename mpllibs::metaparse::transform<P, single_element_map>::type;
template <typename P>
using wrap_into_string =
    typename mpllibs::metaparse::transform<P, single_element_string>::type;
}
}
