#include "optional_ext.h"
#include "catch.hpp"

TEST_CASE("transform") {
    SECTION("with lvalue") {
        optional o(2);
        auto p = o.transform([](int v){ return v*2;});
        REQUIRE(p.value() == 4);
        auto p2 = o.transform([](const int& v){ return v*3;});
        REQUIRE(p2.value() == 6);
    }

    SECTION("with rvalue") {
        optional<int> o(3);
        auto p = std::move(o).transform([](int&& mv) {
            auto tmp = std::move(mv);
            tmp = tmp * 2;
            return tmp;
            });
        REQUIRE(p.value() == 6);
    }

    SECTION("with no value") {
        optional<int> o;
        auto p = o.transform([](int v){ return v*2;});
        REQUIRE(p.has_value() == false);
    }

    SECTION("can't modify value") {
        //Doesn't compile:
        //optional o(2);
        //auto p = o.transform([](int& v){ v = 0;});
    }

    SECTION("transform cant use moving function on lvalue") {
        //Doesn't compile:
        //optional o(3);
        //auto p = o.transform([](int&& mv) {
        //    return std::move(mv);
        //    });
    }
}

TEST_CASE("Demo") {
    optional o(2);
    auto p = o.transform([](auto v) { return v*2;})
        .transform([](auto v) { return v+1;})
        .transform([](auto v) { return v*4;});
    REQUIRE(p.value() == 20);
}
