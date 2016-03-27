#pragma once

#include "meta_parser.h"

#include "boost/mpl/string.hpp"

#include <array>
#include <utility>

namespace genrile {
namespace metaparse {

//  base
template <typename T>
class ConstexprValue {
public:
    using value_type = T;
    constexpr ConstexprValue(value_type v): v(v) {}
//    constexpr value_type get() const {return v;}
    constexpr operator value_type() const {return v;}
private:
    value_type v;
};

//  constexpr wrapped types
class Boolean : public ConstexprValue<bool> {
public:
    using ConstexprValue<bool>::ConstexprValue;
    using type = Boolean;
};

class Integer : public ConstexprValue<int> {
public:
    using ConstexprValue<int>::ConstexprValue;
    using type = Integer;
};

class Real : public ConstexprValue<double> {
public:
    using ConstexprValue<double>::ConstexprValue;
    using type = Real;
};

//  transform type trait
template<typename T>
struct constexpr_transform_trait {
    using type = T;
    static constexpr type run() {
        return type();
    }
};

template<>
struct constexpr_transform_trait<std::true_type> {
    using type = Boolean;
    static constexpr type run() {
        return true;
    }
};

template<>
struct constexpr_transform_trait<std::false_type> {
    using type = Boolean;
    static constexpr type run() {
        return false;
    }
};

constexpr int digit_to_int(char c) {
    return c - '0';
}

template <typename MetaString>
struct to_c_string {
private:
    static constexpr auto length = boost::mpl::size<MetaString>::value;
    template <size_t... indices>
    static constexpr std::array<char, length + 1> run(
        const std::index_sequence<indices...>&) {
        return std::array<char, length + 1>{{
            boost::mpl::at_c<MetaString, indices>::type::value..., '\0'
        }};
    }
public:
    static constexpr std::array<char, length + 1> run() {
        return run(std::make_index_sequence<length>{});
    }
};

template <char... contents>
struct constexpr_transform_trait<string<contents...>>
    : public to_c_string<string<contents...>> {};

template<typename T>
struct constexpr_transform_trait<boxed_integer_string<T>> {
    using type = Integer;
    static constexpr type run() {
        constexpr auto str = to_c_string<T>::run();

        auto pos = 0;

        bool negative = false;
        if (str[pos] == '-') {
            negative = true;
            pos += 1;
        }

        Integer::value_type ret = 0;
        while (str[pos]) {
            ret = ret * 10 + digit_to_int(str[pos]);
            pos += 1;
        }

        if (negative)
            ret *= -1;

        return ret;
    }
};

template<typename T>
struct constexpr_transform_trait<boxed_real_string<T>> {
    using type = Real;
    static constexpr type run() {
        //  TODO magic
        return 0;
    }
};

}
}
