#pragma once

namespace genrile {

class Integer;
class Real;
class Boolean;
class String;
class Array;
class Object;
class Null;

enum class JsonType {
    integer,
    real,
    boolean,
    string,
    array,
    object,
    null,
};

template <JsonType TYPE>
struct JsonCastTrait {};
template <>
struct JsonCastTrait<JsonType::integer> {
    using type = Integer;
};
template <>
struct JsonCastTrait<JsonType::real> {
    using type = Real;
};
template <>
struct JsonCastTrait<JsonType::boolean> {
    using type = Boolean;
};
template <>
struct JsonCastTrait<JsonType::string> {
    using type = String;
};
template <>
struct JsonCastTrait<JsonType::array> {
    using type = Array;
};
template <>
struct JsonCastTrait<JsonType::object> {
    using type = Object;
};
template <>
struct JsonCastTrait<JsonType::null> {
    using type = Null;
};
}
