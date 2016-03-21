#pragma once

#include "linked_list.h"

#include <string>
#include <sstream>
#include <iostream>

namespace genrile {

class ParseError : public std::exception {
public:
    ParseError(const char* s)
            : s(s) {
    }
    const char* what() const noexcept override {
        return s;
    }

private:
    const char* s;
};

class EofError : public ParseError {
public:
    EofError()
            : ParseError("unexpected eof") {
    }
};

class StreamPosSaver {
public:
    StreamPosSaver(std::istream& is);
    void set_unwind(bool b);
    virtual ~StreamPosSaver() noexcept;

    virtual void set_successful();

    bool peek_is() const {
        return false;
    }
    template <typename... Ts>
    bool peek_is(char a, Ts&&... ts) const {
        return is.peek() == a || peek_is(std::forward<Ts>(ts)...);
    }

    std::istream& is;
    std::streampos pos;
    bool unwind{true};
};

class ParseHelper : public StreamPosSaver {
public:
    ParseHelper(std::istream& is);
    char peek() const;
    void get();
    std::string get_parsed();

private:
    std::string got;
};

constexpr uint32_t requires_in_range(uint32_t i, uint32_t length) {
    return length <= i ? throw ParseError("out of range") : i;
}

class MetaString {
public:
    constexpr MetaString(const char* arr, uint32_t N)
            : data(arr)
            , length(N) {
    }

    template <uint32_t N>
    constexpr MetaString(const char(&arr)[N])
            : MetaString(arr, N - 1) {
    }

    constexpr char operator[](uint32_t i) const {
        return requires_in_range(i, length), data[i];
    }

    constexpr operator const char*() const {
        return data;
    }

    constexpr uint32_t size() const {
        return length;
    }

private:
    const char* data;
    uint32_t length;
};

inline std::ostream& operator<<(std::ostream& os, const MetaString& ms) {
    for (uint32_t i = 0; i != ms.size(); ++i) {
        os << ms[i];
    }
    return os;
}

enum class ParseStateType {
    good,
    expected_digit,
    eof,
    fail,
};

class ParseState : public MetaString {
public:
    constexpr ParseState(const MetaString& ms, uint32_t offset = 0)
            : MetaString(ms)
            , offset(offset) {
    }

    constexpr ParseState(const ParseState& ps, uint32_t offset = 0)
            : MetaString(ps)
            , offset(ps.offset + offset) {
    }

    constexpr ParseState(const char* arr, uint32_t N)
            : MetaString(arr, N) {
    }

    template <uint32_t N>
    constexpr ParseState(const char(&arr)[N])
            : ParseState(arr, N) {
    }

    constexpr char operator[](uint32_t i) const {
        return MetaString::operator[](i + offset);
    }

    constexpr uint32_t size() const {
        return MetaString::size() - offset;
    }

    constexpr std::pair<bool, ParseState> match(const MetaString& ms) const {
        uint32_t offset{0};
        for (; offset < ms.size(); ++offset)
            if (operator[](offset) != ms[offset])
                return std::make_pair(false, *this);
        return std::make_pair(true, ParseState(*this, offset));
    }

    constexpr operator const char*() const {
        return MetaString::operator const char*() + offset;
    }

    constexpr uint32_t off() const {
        return offset;
    }

private:
    uint32_t offset{0};
};

/*
class LinkedString : public LinkedList<MetaString> {
public:
    constexpr LinkedString(const MetaString& ms,
                           const LinkedString* const next = nullptr)
            : LinkedList<MetaString>(ms, next) {
    }

    constexpr LinkedString(const char* arr,
                           uint32_t N,
                           const LinkedString* const next = nullptr)
            : LinkedList<MetaString>(MetaString(arr, N), next) {
    }

    template <uint32_t N>
    constexpr LinkedString(const char(&arr)[N],
                           const LinkedString* const next = nullptr)
            : LinkedString(arr, N - 1, next) {
    }

    constexpr char operator[] (uint32_t i) const {
        return contents().size() <= i
                   ? next()->contents().operator[](i - contents().size())
                   : contents()[i];
    }

    constexpr uint32_t size() const {
        return contents().size() + (next() ? next()->size() : 0);
    }
};

inline std::ostream& operator<<(std::ostream& os, const LinkedString& ls) {
    for (uint32_t i = 0; i != ls.size(); ++i) {
        os << ls[i];
    }
    return os;
}
*/

constexpr bool char_is_nonzero_digit(char c) {
    return '1' <= c && c <= '9';
}
constexpr bool char_is_digit(char c) {
    return c == '0' || char_is_nonzero_digit(c);
}
constexpr int literal_value(char c) {
    return c - '0';
}

constexpr bool char_is(char c) {
    return false;
}

template <typename... Ts>
constexpr bool char_is(char c, char x, Ts&&... ts) {
    return c == x || char_is(c, std::forward<Ts>(ts)...);
}

bool match_string(std::istream& is, const std::string& str);
}
