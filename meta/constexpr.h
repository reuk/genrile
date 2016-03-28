#pragma once

#include "meta_parser.h"

#include <mpllibs/metaparse/string.hpp>

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
    using return_type = T;
    static constexpr return_type value = return_type();
};

template <>
struct constexpr_transform_trait<std::true_type> {
    using return_type = Boolean;
    static constexpr return_type value = true;
};

template <>
struct constexpr_transform_trait<std::false_type> {
    using return_type = Boolean;
    static constexpr return_type value = false;
};

constexpr int digit_to_int(char c) {
    return c - '0';
}

template <char... contents>
struct constexpr_transform_trait<string<contents...>>
    : public boost::mpl::c_str<string<contents...>>::type {};

template <typename T>
struct constexpr_transform_trait<boxed_integer_string<T>> {
    using return_type = Integer;
    static constexpr return_type run() {
        constexpr auto str = boost::mpl::c_str<T>::type::value;

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
    static constexpr return_type value = run();
};

template <typename T>
struct constexpr_transform_trait<boxed_real_string<T>> {
    using return_type = Real;

    static constexpr bool isdigit(char c) {
        return '0' <= c && c <= '9';
    }

    static constexpr return_type run() {
        //  taken from
        //  opensource.apple.com/source/tcl/tcl-10/tcl/compat/strtod.c

        constexpr int maxExponent = 511;

        constexpr double powersOf10[] = {
            10., 100., 1.0e4, 1.0e8, 1.0e16, 1.0e32, 1.0e64, 1.0e128, 1.0e256};

        constexpr auto string = boost::mpl::c_str<T>::type::value;

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

    static constexpr return_type value = run();
};
}
}
