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
        auto p = optional<int>(3).transform([](int&& mv) { return mv * 2; });
        REQUIRE(p.value() == 6);
    }

    SECTION("with no value") {
        optional<int> o;
        auto p = o.transform([](int v){ return v*2;});
        REQUIRE(p.has_value() == false);
    }

    SECTION("can't use operation with non const parameter") {
        optional o(2);
        //Doesn't compile:
        //auto p = o.transform([](int&){ return 0;});
    }

    SECTION("can't use operaiton with rvalue parameter on lvalue") {
        optional o(3);
        //Doesn't compile:
        //auto p = o.transform([](int&&) { return 0; });
    }

    SECTION("rvalue optional can also use operation taking lvalue") {
        auto p = optional(2).transform([](const int& v){ return v*2;});
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
