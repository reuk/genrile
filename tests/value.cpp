#include "catch.hpp"
#include "value.h"

#include <iostream>

using namespace JsonParser;

TEST_CASE("create some nested values", "[Value]") {
    Array array;
    array.push_back(5);
    array.push_back(1.234);
    array.push_back(true);
    array.push_back("hello");
    std::cout << array.to_json() << std::endl;
    //  [5, 1.234, true, "hello"]

    Object object;
    object["a"] = 5;
    object["b"] = 1.234;
    object["x"] = true;
    object["y"] = "hello world";
    std::cout << object.to_json() << std::endl;
    //  {"a": 5, "b": 1.234, "x": true, "y": "hello world"}

    array[2] = object;
    std::cout << array.to_json() << std::endl;
    //  [5, 1.234, {"a": 5, "b": 1.234, "x": true, "y": "hello world"}, "hello"]

    object["array"] = array;
    std::cout << object.to_json() << std::endl;
    //  {"a": 5, "array": [5, 1.234, {"a": 5, "b": 1.234, "x": true, "y": "hello world"}, "hello"], "b": 1.234, "x": true, "y": "hello world"}
}
