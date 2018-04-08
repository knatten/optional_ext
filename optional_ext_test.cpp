#include "optional_ext.h"
#include "catch.hpp"

TEST_CASE("transform with lvalue") {
    optional o(2);
    auto p = o.transform([](int v){ return v*2;});
    REQUIRE(p.value() == 4);
    auto p2 = o.transform([](const int& v){ return v*3;});
    REQUIRE(p2.value() == 6);
}

TEST_CASE("transform can't modify value") {
    optional o(2);
    //Doesn't compile:
    //auto p = o.transform([](int& v){ v = 0;});
}

TEST_CASE("transform without value") {
    optional<int> o;
    auto p = o.transform([](int v){ return v*2;});
    REQUIRE(p.has_value() == false);
}

TEST_CASE("transform with rvalue") {
    optional<int> o(3);
    auto p = std::move(o).transform([](int&& mv) {
        auto tmp = std::move(mv);
        tmp = tmp * 2;
        return tmp;
        });
    REQUIRE(p.value() == 6);
}

TEST_CASE("transform cant use moving function on lvalue") {
    optional o(3);
    //Doesn't compile:
    /* auto p = o.transform([](int&& mv) { */
    /*     return std::move(mv); */
    /*     }); */
}

TEST_CASE("Demo") {
    optional o(2);
    auto p = o.transform([](auto v) { return v*2;})
        .transform([](auto v) { return v+1;})
        .transform([](auto v) { return v*4;});
    REQUIRE(p.value() == 20);
}
