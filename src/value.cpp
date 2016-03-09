#include "parse_helpers.h"
#include "value.h"

#include <sstream>
#include <regex>

class Bracketer final {
public:
    Bracketer(std::ostream& os, const char* open = "{", const char* close = "}")
            : os(os)
            , close(close) {
        os << open;
    }
    ~Bracketer() noexcept {
        os << close;
    }

private:
    std::ostream& os;
    const char* close;
};

std::ostream& operator<<(std::ostream& os, const Genrile::Variadic& v) {
    return v->write_json_to_stream(os);
}

std::ostream& operator<<(
    std::ostream& os,
    const std::pair<const Genrile::String, Genrile::Variadic>& v) {
    v.first.write_json_to_stream(os) << ": ";
    return v.second->write_json_to_stream(os);
}

class comma_separate_iterator
    : public std::iterator<std::output_iterator_tag, void, void, void, void> {
public:
    comma_separate_iterator(std::ostream& os, const char* delim = ", ")
            : os(os)
            , delim(delim) {
    }

    template <typename T>
    comma_separate_iterator& operator=(const T& t) {
        if (!first && delim)
            os << delim;
        os << t;
        first = false;
        return *this;
    }

    comma_separate_iterator& operator*() {
        return *this;
    }

    comma_separate_iterator& operator++() {
        return *this;
    }

    comma_separate_iterator& operator++(int) {
        return *this;
    }

private:
    std::ostream& os;
    const char* delim;
    bool first{true};
};

template <typename It>
static std::ostream& comma_separate(std::ostream& os, It begin, It end) {
    std::copy(begin, end, comma_separate_iterator(os, ", "));
    return os;
}

//----------------------------------------------------------------------------//

namespace Genrile {

Integer& Value::get_integer() {
    return get<Type::integer>();
}
const Integer& Value::get_integer() const {
    return get<Type::integer>();
}
Real& Value::get_real() {
    return get<Type::real>();
}
const Real& Value::get_real() const {
    return get<Type::real>();
}
Boolean& Value::get_boolean() {
    return get<Type::boolean>();
}
const Boolean& Value::get_boolean() const {
    return get<Type::boolean>();
}
String& Value::get_string() {
    return get<Type::string>();
}
const String& Value::get_string() const {
    return get<Type::string>();
}
Array& Value::get_array() {
    return get<Type::array>();
}
const Array& Value::get_array() const {
    return get<Type::array>();
}
Object& Value::get_object() {
    return get<Type::object>();
}
const Object& Value::get_object() const {
    return get<Type::object>();
}
Null& Value::get_null() {
    return get<Type::null>();
}
const Null& Value::get_null() const {
    return get<Type::null>();
}

std::string Value::to_json() const {
    std::stringstream ss;
    write_json_to_stream(ss);
    return ss.str();
}

//----------------------------------------------------------------------------//

Variadic::Variadic(const Integer& t)
        : std::unique_ptr<Value>(std::make_unique<Integer>(t)) {
}
Variadic::Variadic(const Real& t)
        : std::unique_ptr<Value>(std::make_unique<Real>(t)) {
}
Variadic::Variadic(const Boolean& t)
        : std::unique_ptr<Value>(std::make_unique<Boolean>(t)) {
}
Variadic::Variadic(const String& t)
        : std::unique_ptr<Value>(std::make_unique<String>(t)) {
}
Variadic::Variadic(const Array& t)
        : std::unique_ptr<Value>(std::make_unique<Array>(t)) {
}
Variadic::Variadic(const Object& t)
        : std::unique_ptr<Value>(std::make_unique<Object>(t)) {
}
Variadic::Variadic(const Null& t)
        : std::unique_ptr<Value>(std::make_unique<Null>(t)) {
}

Variadic::Variadic(Integer::value_type t)
        : Variadic(Integer(t)) {
}
Variadic::Variadic(Real::value_type t)
        : Variadic(Real(t)) {
}
Variadic::Variadic(Boolean::value_type t)
        : Variadic(Boolean(t)) {
}
Variadic::Variadic(const String::value_type& t)
        : Variadic(String(t)) {
}
Variadic::Variadic(const char* t)
        : Variadic(String(t)) {
}

bool Variadic::operator==(const Variadic& v) const {
    auto& this_value = *(*this);
    auto& that_value = *v;
    auto this_type = this_value.get_type();
    if (that_value.get_type() == this_type) {
        switch (this_type) {
            case Value::Type::integer:
                return this_value.get_integer() == that_value.get_integer();
            case Value::Type::real:
                return this_value.get_real() == that_value.get_real();
            case Value::Type::boolean:
                return this_value.get_boolean() == that_value.get_boolean();
            case Value::Type::string:
                return this_value.get_string() == that_value.get_string();
            case Value::Type::array:
                return this_value.get_array() == that_value.get_array();
            case Value::Type::object:
                return this_value.get_object() == that_value.get_object();
            case Value::Type::null:
                return true;
        }
    }
    return false;
}
bool Variadic::operator!=(const Variadic& v) const {
    return !operator==(v);
}

//----------------------------------------------------------------------------//

Variadic Integer::clone() const {
    return Variadic(*this);
}
std::ostream& Integer::write_json_to_stream(std::ostream& os) const {
    return os << value;
}
void Integer::read_json_from_stream(std::istream& is) {
    ParseHelper helper(is);
    if (helper.peek() == '0') {
        helper.get();
        value = std::stoi(helper.get_parsed());
        return;
    }
    if (helper.peek() == '-')
        helper.get();
    if (helper.peek() == '1' || helper.peek() == '2' || helper.peek() == '3' ||
        helper.peek() == '4' || helper.peek() == '5' || helper.peek() == '6' ||
        helper.peek() == '7' || helper.peek() == '8' || helper.peek() == '9') {
        helper.get();
    } else {
        throw ParseError(
            0,
            0,
            "character immediately following '-' is not a non-zero integer");
    }

    while (helper.peek() == '0' || helper.peek() == '1' ||
           helper.peek() == '2' || helper.peek() == '3' ||
           helper.peek() == '4' || helper.peek() == '5' ||
           helper.peek() == '6' || helper.peek() == '7' ||
           helper.peek() == '8' || helper.peek() == '9') {
        helper.get();
    }

    value = std::stoi(helper.get_parsed());
}

//----------------------------------------------------------------------------//

Variadic Real::clone() const {
    return Variadic(*this);
}
std::ostream& Real::write_json_to_stream(std::ostream& os) const {
    return os << value;
}
void Real::read_json_from_stream(std::istream& is) {
}

//----------------------------------------------------------------------------//

Variadic Boolean::clone() const {
    return Variadic(*this);
}
std::ostream& Boolean::write_json_to_stream(std::ostream& os) const {
    return os << (value ? "true" : "false");
}
void Boolean::read_json_from_stream(std::istream& is) {
    if (match_string(is, "true")) {
        value = true;
        return;
    } else if (match_string(is, "false")) {
        value = false;
        return;
    }
    throw ParseError(0, 0, "value is not 'true' or 'false'");
}

//----------------------------------------------------------------------------//

Variadic String::clone() const {
    return Variadic(*this);
}
std::ostream& String::write_json_to_stream(std::ostream& os) const {
    Bracketer bracketer(os, "\"", "\"");
    return os << value;
}
void String::read_json_from_stream(std::istream& is) {
    if (!match_string(is, "\""))
        throw ParseError(0, 0, "expected opening double-quotes");
    ParseHelper helper(is);
    while (helper.peek() != '"')
        helper.get();
    value = helper.get_parsed();
    helper.get();
    helper.set_successful();
}

String::String(const char* c)
        : PrimitiveJsonValue<std::string, Type::string>(c) {
}
String::String(const std::string& s)
        : PrimitiveJsonValue<std::string, Value::Type::string>(s) {
}
String::String(std::string&& s)
        : PrimitiveJsonValue<std::string, Value::Type::string>(std::move(s)) {
}

//----------------------------------------------------------------------------//

Variadic Array::clone() const {
    return Variadic(*this);
}
std::ostream& Array::write_json_to_stream(std::ostream& os) const {
    Bracketer bracketer(os, "[", "]");
    return comma_separate(os, cbegin(), cend());
}
void Array::read_json_from_stream(std::istream& is) {
}

Array::Array(const Array& rhs)
        : PrimitiveJsonValue(value_type(rhs.size())) {
    std::transform(rhs.begin(),
                   rhs.end(),
                   value.begin(),
                   [](const auto& i) { return i->clone(); });
}
Array& Array::operator=(const Array& rhs) {
    return *this;
}

Array::value_type::reference Array::at(value_type::size_type pos) {
    return value.at(pos);
}
Array::value_type::const_reference Array::at(value_type::size_type pos) const {
    return value.at(pos);
}
Array::value_type::reference Array::operator[](value_type::size_type pos) {
    return value[pos];
}
Array::value_type::const_reference Array::operator[](
    value_type::size_type pos) const {
    return value[pos];
}
Array::value_type::reference Array::front() {
    return value.front();
}
Array::value_type::const_reference Array::front() const {
    return value.front();
}
Array::value_type::reference Array::back() {
    return value.back();
}
Array::value_type::const_reference Array::back() const {
    return value.back();
}
Array::value_type::iterator Array::begin() {
    return value.begin();
}
Array::value_type::const_iterator Array::begin() const {
    return value.begin();
}
Array::value_type::const_iterator Array::cbegin() const {
    return value.cbegin();
}
Array::value_type::reverse_iterator Array::rbegin() {
    return value.rbegin();
}
Array::value_type::const_reverse_iterator Array::rbegin() const {
    return value.rbegin();
}
Array::value_type::const_reverse_iterator Array::crbegin() const {
    return value.crbegin();
}
Array::value_type::iterator Array::end() {
    return value.end();
}
Array::value_type::const_iterator Array::end() const {
    return value.end();
}
Array::value_type::const_iterator Array::cend() const {
    return value.cend();
}
Array::value_type::reverse_iterator Array::rend() {
    return value.rend();
}
Array::value_type::const_reverse_iterator Array::rend() const {
    return value.rend();
}
Array::value_type::const_reverse_iterator Array::crend() const {
    return value.crend();
}

bool Array::empty() const {
    return value.empty();
}
Array::value_type::size_type Array::size() const {
    return value.size();
}

void Array::clear() {
    value.clear();
}

Array::value_type::iterator Array::insert(value_type::const_iterator pos,
                                          const Variadic& v) {
    return value.insert(pos, v->clone());
}
Array::value_type::iterator Array::erase(value_type::const_iterator pos) {
    return value.erase(pos);
}
Array::value_type::iterator Array::erase(value_type::const_iterator first,
                                         value_type::const_iterator last) {
    return value.erase(first, last);
}

void Array::push_back(const Variadic& v) {
    value.push_back(v->clone());
}
void Array::pop_back() {
    value.pop_back();
}

//----------------------------------------------------------------------------//

Variadic Object::clone() const {
    return Variadic(*this);
}
std::ostream& Object::write_json_to_stream(std::ostream& os) const {
    Bracketer bracketer(os);
    return comma_separate(os, cbegin(), cend());
}
void Object::read_json_from_stream(std::istream& is) {
}

Object::Object(const Object& rhs) {
    for (const auto& i : rhs) {
        value[i.first] = i.second->clone();
    }
}
Object& Object::operator=(const Object& rhs) {
    return *this;
}

Object::value_type::iterator Object::begin() {
    return value.begin();
}
Object::value_type::const_iterator Object::begin() const {
    return value.begin();
}
Object::value_type::iterator Object::end() {
    return value.end();
}
Object::value_type::const_iterator Object::end() const {
    return value.end();
}
Object::value_type::reverse_iterator Object::rbegin() {
    return value.rbegin();
}
Object::value_type::const_reverse_iterator Object::rbegin() const {
    return value.rbegin();
}
Object::value_type::reverse_iterator Object::rend() {
    return value.rend();
}
Object::value_type::const_reverse_iterator Object::rend() const {
    return value.rend();
}
Object::value_type::const_iterator Object::cbegin() const {
    return value.cbegin();
}
Object::value_type::const_iterator Object::cend() const {
    return value.cend();
}
Object::value_type::const_reverse_iterator Object::crbegin() const {
    return value.crbegin();
}
Object::value_type::const_reverse_iterator Object::crend() const {
    return value.crend();
}

bool Object::empty() const {
    return value.empty();
}
Object::value_type::size_type Object::size() const {
    return value.size();
}

void Object::clear() {
    value.clear();
}

Object::value_type::mapped_type& Object::at(const String& k) {
    return value.at(k);
}
const Object::value_type::mapped_type& Object::at(const String& k) const {
    return value.at(k);
}
Variadic& Object::operator[](const String& k) {
    return value[k];
}

void Object::erase(value_type::iterator position) {
    value.erase(position);
}
void Object::erase(value_type::iterator first, value_type::iterator last) {
    value.erase(first, last);
}

Object::value_type::iterator Object::find(const value_type::key_type& v) {
    return value.find(v);
}
Object::value_type::const_iterator Object::find(
    const value_type::key_type& v) const {
    return value.find(v);
}

//----------------------------------------------------------------------------//

Value::Type Null::get_type() const {
    return type;
}
Variadic Null::clone() const {
    return Variadic(*this);
}
std::ostream& Null::write_json_to_stream(std::ostream& os) const {
    return os << "null";
}
void Null::read_json_from_stream(std::istream& is) {
}

//----------------------------------------------------------------------------//
}
