#pragma once

#include <string>

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

class ParseHelper final {
public:
    ParseHelper(std::istream& is);
    ~ParseHelper();
    char peek() const;
    void get();
    void unget();
    void set_successful();
    std::string get_parsed();

    bool peek_is() const {
        return false;
    }
    template <typename... Ts>
    bool peek_is(char a, Ts&&... ts) const {
        return peek() == a || peek_is(std::forward<Ts>(ts)...);
    }

private:
    std::istream& is;
    std::string got;
};

bool match_string(std::istream& is, const std::string& str);
}
