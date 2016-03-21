#pragma once

#include "parse_helpers.h"

namespace Genrile {

/*
namespace Meta {

template <typename T>
class ParserOutput {
public:
constexpr ParserOutput(const T& t, const ParseState& ps)
        : t(t)
        , parse_state(ps) {
}

constexpr ParserOutput(ParseStateType pst)
        : parse_state_type(pst) {
}

constexpr ParserOutput(const T& t, const ParseState& ps, ParseStateType pst)
        : t(t)
        , parse_state(ps)
        , parse_state_type(pst) {
}

constexpr bool is_good() const {
    return parse_state_type == ParseStateType::good;
}

constexpr ParseState get_parse_state() const {
    return parse_state;
}

constexpr T get_value() const {
    return t;
}

constexpr ParseStateType get_parse_state_type() const {
    return parse_state_type;
}

private:
T t;
ParseState parse_state;
ParseStateType parse_state_type{ParseStateType::good};
};

constexpr ParserOutput<int> parse_integer(
const ParseState& ps) {
uint32_t offset{0};
bool negative{false};
int ret{0};
ParseStateType state_type{ParseStateType::good};

if (char_is(ps[offset], '-')) {
    negative = true;
    offset += 1;
}

if (char_is(ps[offset], '0')) {
    ret = 0;
    offset += 1;
} else if (char_is_nonzero_digit(ps[offset])) {
    ret = literal_value(ps[offset]);
    offset += 1;
    while (char_is_digit(ps[offset])) {
        ret *= 10;
        ret += literal_value(ps[offset]);
        offset += 1;
    }
} else {
    state_type = ParseStateType::expected_digit;
}

if (negative)
    ret *= -1;

return ParserOutput<int>(ret);
}

constexpr ParserOutput<double> parse_real(const ParseState& ps) {
uint32_t offset{0};
bool negative{false};
double ret{0};

if (char_is(ps[offset], '-')) {
    negative = true;
    offset += 1;
}

if (char_is(ps[offset], '0')) {
    ret = 0;
    offset += 1;
} else if (char_is_nonzero_digit(ps[offset])) {
    ret = literal_value(ps[offset]);
    offset += 1;
    while (char_is_digit(ps[offset])) {
        ret *= 10;
        ret += literal_value(ps[offset]);
        offset += 1;
    }
} else {
    throw ParseError("expected digit");
}

if (!char_is(ps[offset], '.', 'e', 'E')) {
    throw ParseError("real must contain '.', 'e', or 'E'");
}

if (char_is(ps[offset], '.')) {
    offset += 1;
    if (!char_is_digit(ps[offset])) {
        throw ParseError("decimal point must be followed by a digit");
    }
}

return std::make_pair(ret, ParseState(ps,offset));
}

constexpr ParserOutput<bool> parse_boolean(const ParseState& ps) {
{
    auto match = ps.match("true");
    if (match.first)
        return std::make_pair(true, match.second);
}

{
    auto match = ps.match("false");
    if (match.first)
        return std::make_pair(false, match.second);
}

throw ParseError("string is not 'true' or 'false'");
}

class Null {
public:
using type = Null;

constexpr bool operator==(const Null&) const {
    return true;
}
};

constexpr ParserOutput<Null> parse_null(const ParseState& ps) {
{
    auto match = ps.match("null");
    if (match.first)
        return std::make_pair(Null(), match.second);
}

throw ParseError("string is not 'null'");
}

constexpr ParserOutput<MetaString> parse_string(const ParseState& ps) {
auto match = ps.match("\"");
if (! match.first)
    throw ParseError("expected opening double-quotes");
auto tmp = match.second;

uint32_t offset{0};
while (true) {
    if (char_is(tmp[offset], '"')) {
        return std::make_pair(MetaString(tmp, offset),
                              ParseState(tmp, offset + 1));
    } else if (char_is(tmp[offset], '\\')) {
        offset += 1;
        switch (tmp[offset]) {
            case '"':
                offset += 1;
                break;
            case '\\':
                offset += 1;
                break;
            case '/':
                offset += 1;
                break;
            case 'b':
                offset += 1;
                break;
            case 'f':
                offset += 1;
                break;
            case 'n':
                offset += 1;
                break;
            case 'r':
                offset += 1;
                break;
            case 't':
                offset += 1;
                break;
            case 'u': {
                offset += 1;
                uint16_t codepoint = 0;
                for (int i = 0; i != 4; ++i) {
                    auto c = tmp[offset];
                    codepoint <<= 4;
                    codepoint += static_cast<uint16_t>(c);
                    if (c >= '0' && c <= '9') {
                        codepoint -= '0';
                    } else if (c >= 'A' && c <= 'F') {
                        codepoint -= 'A' - 10;
                    } else if (c >= 'a' && c <= 'f') {
                        codepoint -= 'a' - 10;
                    } else {
                        throw ParseError("expected 4 hex digits");
                    }
                }
                break;
            }
            default:
                throw ParseError("unrecognized control sequence");
        }
    } else {
        offset += 1;
    }
}
}

template <uint32_t N>
constexpr auto parse_inner_array(const ParseState& ps,
                             const std::array<int, N>& arr) {
auto match = ps.match("]");
if (match.first)
    return std::make_pair
}

constexpr auto parse_array(const ParseState& ps) {
auto match = ps.match("[");
if (! match.first)
    throw ParseError("expected opening square bracket");
auto tmp = match.second;

//  parse inside
}
}
*/
}
