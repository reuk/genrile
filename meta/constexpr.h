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
    constexpr ConstexprValue(value_type v)
            : v(v) {
    }
    //    constexpr value_type get() const {return v;}
    constexpr operator value_type() const {
        return v;
    }

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
template <typename T>
struct constexpr_transform_trait {
    using type = T;
    static constexpr type run() {
        return type();
    }
};

template <>
struct constexpr_transform_trait<std::true_type> {
    using type = Boolean;
    static constexpr type run() {
        return true;
    }
};

template <>
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
        const std::index_sequence<indices...> &) {
        return std::array<char, length + 1>{
            {boost::mpl::at_c<MetaString, indices>::type::value..., '\0'}};
    }

public:
    static constexpr std::array<char, length + 1> run() {
        return run(std::make_index_sequence<length>{});
    }
};

template <char... contents>
struct constexpr_transform_trait<string<contents...>>
    : public to_c_string<string<contents...>> {};

template <typename T>
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

        return negative ? -ret : ret;
    }
};

template <typename T>
struct constexpr_transform_trait<boxed_real_string<T>> {
    using type = Real;

    static constexpr bool isdigit(char c) {
        return '0' <= c && c <= '9';
    }

    static constexpr type run() {
        //  taken from
        //  opensource.apple.com/source/tcl/tcl-10/tcl/compat/strtod.c

        constexpr int maxExponent = 511;

        constexpr double powersOf10[] = {
            10., 100., 1.0e4, 1.0e8, 1.0e16, 1.0e32, 1.0e64, 1.0e128, 1.0e256};

        constexpr auto string = to_c_string<T>::run();

        auto p_ind = 0;

        auto sign = false;
        if (string[p_ind] == '-') {
            sign = true;
            p_ind += 1;
        } else {
            if (string[p_ind] == '+') {
                p_ind += 1;
            }
            sign = false;
        }

        auto decPt = -1;
        auto mantSize = 0;
        auto c = 0;
        for (;; mantSize += 1) {
            c = string[p_ind];
            if (!isdigit(c)) {
                if ((c != '.') || (decPt >= 0)) {
                    break;
                }
                decPt = mantSize;
            }
            p_ind += 1;
        }

        auto pExp = p_ind;
        p_ind -= mantSize;
        if (decPt < 0) {
            decPt = mantSize;
        } else {
            mantSize -= 1;
        }
        auto fracExp = 0;
        if (mantSize > 18) {
            fracExp = decPt - 18;
            mantSize = 18;
        } else {
            fracExp = decPt - mantSize;
        }
        auto fraction = 0.0;
        if (mantSize == 0) {
            fraction = 0.0;
            p_ind = 0;
            return sign ? -fraction : fraction;
        } else {
            auto frac1 = 0;
            for (; mantSize > 9; mantSize -= 1) {
                c = string[p_ind];
                p_ind += 1;
                if (c == '.') {
                    c = string[p_ind];
                    p_ind += 1;
                }
                frac1 = 10 * frac1 + (c - '0');
            }
            auto frac2 = 0;
            for (; mantSize > 0; mantSize -= 1) {
                c = string[p_ind];
                p_ind += 1;
                if (c == '.') {
                    c = string[p_ind];
                    p_ind += 1;
                }
                frac2 = 10 * frac2 + (c - '0');
            }
            fraction = (1.0e9 * frac1) + frac2;
        }

        auto expSign = false;
        auto exp = 0;
        p_ind = pExp;
        if ((string[p_ind] == 'E') || (string[p_ind] == 'e')) {
            p_ind += 1;
            if (string[p_ind] == '-') {
                expSign = true;
                p_ind += 1;
            } else {
                if (string[p_ind] == '+') {
                    p_ind += 1;
                }
                expSign = false;
            }
            if (!isdigit(string[p_ind])) {
                p_ind = pExp;
                return sign ? -fraction : fraction;
            }
            while (isdigit(string[p_ind])) {
                exp = exp * 10 + (string[p_ind] - '0');
                p_ind += 1;
            }
        }
        if (expSign) {
            exp = fracExp - exp;
        } else {
            exp = fracExp + exp;
        }

        if (exp < 0) {
            expSign = true;
            exp = -exp;
        } else {
            expSign = false;
        }
        if (exp > maxExponent) {
            exp = maxExponent;
            errno = ERANGE;
        }
        auto dblExp = 1.0;
        auto d = powersOf10;
        for (; exp != 0; exp >>= 1, d += 1) {
            if (exp & 01) {
                dblExp *= *d;
            }
        }
        if (expSign) {
            fraction /= dblExp;
        } else {
            fraction *= dblExp;
        }

        return sign ? -fraction : fraction;
    }
};
}
}
