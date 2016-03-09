#include "parse_helpers.h"

#include <sstream>

namespace Genrile {

ParseError::ParseError(int line, int column, const std::string& str)
        : line(line)
        , column(column)
        , str(str) {
}
const char* ParseError::what() const noexcept {
    std::stringstream ss;
    ss << "parse error at line " << line << " column " << column << ": " << str;
    return ss.str().c_str();
}

ParseHelper::ParseHelper(std::istream& is)
        : is(is) {
}
ParseHelper::~ParseHelper() {
    unget();
}
char ParseHelper::peek() const {
    return is.peek();
}
void ParseHelper::get() {
    got.push_back(is.get());
}
void ParseHelper::unget() {
    for (auto i = 0u; i != got.size(); ++i)
        is.unget();
    set_successful();
}
void ParseHelper::set_successful() {
    got = "";
}
std::string ParseHelper::get_parsed() {
    auto ret = got;
    set_successful();
    return ret;
}

bool match_string(std::istream& is, const std::string& str) {
    ParseHelper helper(is);
    for (auto c : str) {
        if (helper.peek() == c) {
            helper.get();
        } else {
            return false;
        }
    }
    helper.set_successful();
    return true;
}
}
