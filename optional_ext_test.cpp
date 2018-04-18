#include "optional_ext.h"
#include "catch.hpp"

using std::string;
using knatten::optional;

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

    SECTION("rvalue optional can also use operation taking lvalue") {
        optional o(2);
        auto p = std::move(o).transform([](const int& v){ return v*2;});
        REQUIRE(p.value() == 4);
    }
}

struct tweet {};

struct author {
    author(string name) : name(name) {}
    string name;
};

optional<tweet> find_first(const string& search_string);

author lookup_author(const tweet& t);

TEST_CASE("Simple demo") {

    SECTION("old style") {
        auto foo = find_first("foo");
        auto foo_author(foo.has_value() ? lookup_author(*foo) : optional<author>());
        REQUIRE((*foo_author).name == "@knatten");
    }

    SECTION("new style") {
        auto foo_author = find_first("foo").transform(lookup_author);
        REQUIRE(foo_author->name == "@knatten");
    }

    SECTION("such functional style") {
        REQUIRE(find_first("foo").transform(lookup_author)->name == "@knatten");
    }
}

optional<tweet> find_first(const string&) {
    return tweet();
}

author lookup_author(const tweet&) {
    return author("@knatten");
}
