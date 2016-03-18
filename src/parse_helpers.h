#pragma once

#include <string>
#include <sstream>

namespace Genrile {

class ParseError : public std::exception {
public:
    ParseError(int line, int column, const std::string& str);
    const char* what() const noexcept override;

private:
    int line;
    int column;
    std::string str;
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

bool match_string(std::istream& is, const std::string& str);
}
