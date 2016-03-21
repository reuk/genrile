#include "catch.hpp"
#include "value.h"

#include <iostream>

using namespace genrile;

TEST_CASE("create some nested values", "[Value]") {
    Array array = {
        1, "hello", false, 2.5,
    };
    array.push_back(5);
    array.push_back(1.234);
    array.push_back(true);
    array.push_back("world");
    std::cout << array.to_json() << std::endl;

    Object object = {
        {"i", false}, {"j", Array({4, 5, 6})},
    };
    object["a"] = 5;
    object["b"] = 1.234;
    object["x"] = true;
    object["y"] = "hello world";
    std::cout << object.to_json() << std::endl;

    array[2] = object;
    std::cout << array.to_json() << std::endl;

    object["array"] = array;
    std::cout << object.to_json() << std::endl;
}
