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
        auto p = optional(3).transform([](int&& mv) { return mv * 2; });
        REQUIRE(p.value() == 6);
    }

    SECTION("with no lvalue") {
        optional<int> o;
        auto p = o.transform([](int v){ return v*2;});
        REQUIRE(p.has_value() == false);
    }

    SECTION("with no rvalue") {
        auto p = optional<int>().transform([](int v){ return v*2;});
        REQUIRE(p.has_value() == false);
    }

    SECTION("lvalue can't use operation with non const lvalue parameter") {
        optional o(2);
        //Doesn't compile:
        //auto p = o.transform([](int&){ return 0;});
    }

    SECTION("lvalue can't use operation with rvalue parameter") {
        optional o(3);
        //Doesn't compile:
        //auto p = o.transform([](int&&) { return 0; });
    }

    SECTION("rvalue can't use operation with non const lvalue parameter") {
        //Doesn't compile:
        //auto p = optional(2).transform([](int&){ return 0;});
    }

    SECTION("rvalue can use operation with const lvalue parameter") {
        auto p = optional(2).transform([](const int& v){ return v*2;});
        REQUIRE(p.value() == 4);
    }
}

TEST_CASE("transform_opt") {
    SECTION("with lvalue") {
        optional o(2);
        auto p = o.transform_opt([](int v){ return optional(v*2);});
        REQUIRE(p.value() == 4);
        auto p2 = o.transform_opt([](int){ return optional<int>();});
        REQUIRE(p2.has_value() == false);
    }

    SECTION("with rvalue") {
        auto p = optional(3).transform_opt([](int&& mv) { return optional(mv * 2); });
        REQUIRE(p.value() == 6);
        auto p2 = optional(4).transform_opt([](int&&) { return optional<int>(); });
        REQUIRE(p2.has_value() == false);
    }

    SECTION("with no lvalue") {
        optional<int> o;
        auto p = o.transform_opt([](int v){ return optional(v*2);});
        REQUIRE(p.has_value() == false);
        auto p2 = o.transform_opt([](int){ return optional<int>();});
        REQUIRE(p2.has_value() == false);
    }

    SECTION("with no rvalue") {
        auto p = optional<int>().transform_opt([](int&& v){ return optional(v*2);});
        REQUIRE(p.has_value() == false);
        auto p2 = optional<int>().transform_opt([](int&&){ return optional<int>();});
        REQUIRE(p2.has_value() == false);
    }

    SECTION("lvalue can't use operation with non const lvalue parameter") {
        optional o(2);
        //Doesn't compile:
        //auto p = o.transform_opt([](int&){ return optional<int>();});
    }

    SECTION("lvalue can't use operation with rvalue parameter") {
        optional o(3);
        //Doesn't compile:
        //auto p = o.transform_opt([](int&&) { return optional<int>(); });
    }

    SECTION("rvalue can't use operation with non const lvalue parameter") {
        //Doesn't compile:
        //auto p = optional(2).transform_opt([](int&){ return optional<int>();});
    }

    SECTION("rvalue can use operation with const lvalue parameter") {
        auto p = optional(2).transform_opt([](const int& v){ return optional(v*2);});
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

TEST_CASE("Demo of transform") {

    SECTION("old style") {
        auto foo = find_first("foo");
        auto foo_author(foo.has_value() ? lookup_author(*foo) : optional<author>());
        REQUIRE(foo_author->name == "@knatten");
    }

    SECTION("new style") {
        auto foo_author = find_first("foo").transform(lookup_author);
        REQUIRE(foo_author->name == "@knatten");
    }

    SECTION("such oneliner") {
        REQUIRE(find_first("foo").transform(lookup_author)->name == "@knatten");
    }
}

optional<tweet> tweet_replied_to(const tweet& t);

TEST_CASE("Demo of transform_opt") {

    SECTION("old style") {
        auto foo = find_first("foo");
        auto foo_replied_to(foo.has_value() ? tweet_replied_to(*foo) : optional<tweet>());
        REQUIRE(foo_replied_to.has_value() == true);
    }

    SECTION("new style") {
        auto foo_replied_to = find_first("foo").transform_opt(tweet_replied_to);
        REQUIRE(foo_replied_to.has_value() == true);
    }

    SECTION("such oneliner") {
        REQUIRE(find_first("foo").transform_opt(tweet_replied_to).has_value() == true);
    }
}

TEST_CASE("Combined demo") {

    SECTION("old style") {
        auto foo = find_first("foo");
        auto foo_replied_to(foo.has_value() ? tweet_replied_to(*foo) : optional<tweet>());
        auto orig_author(foo_replied_to.has_value() ? lookup_author(*foo_replied_to) : optional<author>());
        REQUIRE(orig_author->name == "@knatten");
    }

    SECTION("new style") {
        auto orig_author = find_first("foo")
            .transform_opt(tweet_replied_to)
            .transform(lookup_author);
        REQUIRE(orig_author->name == "@knatten");
    }

    SECTION("such oneliner") {
        REQUIRE(find_first("foo").transform_opt(tweet_replied_to).transform(lookup_author)->name == "@knatten");
    }
}

optional<tweet> find_first(const string&) {
    return tweet();
}

author lookup_author(const tweet&) {
    return author("@knatten");
}

optional<tweet> tweet_replied_to(const tweet&) {
    return tweet();
}

