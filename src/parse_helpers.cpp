#include "parse_helpers.h"

#include <iostream>

namespace genrile {

StreamPosSaver::StreamPosSaver(std::istream& is)
        : is(is)
        , pos(is.tellg()) {
}
void StreamPosSaver::set_unwind(bool b) {
    unwind = b;
}
StreamPosSaver::~StreamPosSaver() noexcept {
    if (unwind) {
        is.clear();
        is.seekg(pos);
    }
}
void StreamPosSaver::set_successful() {
    set_unwind(false);
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
    set_successful();
    return got;
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
