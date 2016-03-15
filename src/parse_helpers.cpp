#include "parse_helpers.h"

namespace Genrile {

StreamPosSaver::StreamPosSaver(std::istream& is)
        : is(is)
        , pos(is.tellg()) {
}
void StreamPosSaver::set_unwind(bool b) {
    unwind = b;
}
StreamPosSaver::~StreamPosSaver() noexcept {
    if (unwind)
        is.seekg(pos);
}
void StreamPosSaver::set_successful() {
    set_unwind(false);
}

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
        : StreamPosSaver(is) {
}
char ParseHelper::peek() const {
    return is.peek();
}
void ParseHelper::get() {
    got.push_back(is.get());
}
std::string ParseHelper::get_parsed() {
    auto ret = got;
    set_successful();
    return ret;
}

void ParseHelper::set_successful() {
    StreamPosSaver::set_successful();
}

bool match_string(std::istream& is, const std::string& str) {
    StreamPosSaver saver(is);
    for (auto c : str) {
        if (is.peek() == c) {
            is.get();
        } else {
            return false;
        }
    }
    saver.set_successful();
    return true;
}
}
