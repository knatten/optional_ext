#include "optional_ext.h"
#include "catch.hpp"

using std::string;
using knatten::optional;

TEST_CASE("transform") {
    SECTION("with lvalue") {
        optional o(2);
        auto p = o.transform([](int& v){ return v*2;});
        REQUIRE(p.value() == 4);

        auto p2 = o.transform([](const int& v){ return v*3;});
        REQUIRE(p2.value() == 6);

        //Can't use these, doesn't compile
        //o.transform([](int&){ return 0; });
        //o.transform([](int&&) { return 0; });
    }
    SECTION("with const lvalue") {
        const optional o(2);
        auto p = o.transform([](const int& v){ return v*3;});
        REQUIRE(p.value() == 6);

        //Can't use these, doesn't compile
        //o.transform([](int& v){ return 0;});
        //o.transform([](int&){ return 0; });
        //o.transform([](const int&&) { return 0; });
    }

    SECTION("with rvalue") {
        auto p = optional(3).transform([](int&& mv) { return mv * 2; });
        REQUIRE(p.value() == 6);

        auto p2 = optional(3).transform([](const int&& v){ return v*3;});
        REQUIRE(p2.value() == 9);

        auto p3 = optional(3).transform([](const int& v){ return v*4;});
        REQUIRE(p3.value() == 12);

        //Can't use these, doesn't compile
        //optional<int>(3).transform([](int& v){ return v*4;});
    }

    SECTION("with const rvalue") {
        auto p = static_cast<const optional<int>&&>(optional<int>(2)).transform([](const int& mv){ return mv * 2;});
        REQUIRE(p.value() == 4);

        //Can't use these, doesn't compile
        //static_cast<const optional<int>&&>(optional<int>(2)).transform([](int& mv){ return mv * 2;});
        //static_cast<const optional<int>&&>(optional<int>(2)).transform([](int&& mv){ return mv * 2;});
        //static_cast<const optional<int>&&>(optional<int>(2)).transform([](const int&& mv){ return mv * 2;});
    }

    SECTION("with no value") {
        optional<int> o;
        auto p = o.transform([](int v){ return v*2;});
        REQUIRE(p.has_value() == false);

        const optional<int> co;
        auto p2 = o.transform([](int v){ return v*2;});
        REQUIRE(p2.has_value() == false);

        auto p3 = optional<int>().transform([](int v){ return v*2;});
        REQUIRE(p3.has_value() == false);

        auto p4 = static_cast<const optional<int>&&>(optional<int>()).transform([](int v){ return v*2;});
        REQUIRE(p4.has_value() == false);
    }
}

TEST_CASE("transform_opt") {
    SECTION("with lvalue") {
        optional o(2);
        auto p = o.transform_opt([](int& v){ return optional(v*2);});
        REQUIRE(p.value() == 4);

        auto p2 = o.transform_opt([](const int& v){ return optional(v*3);});
        REQUIRE(p2.value() == 6);

        //Can't use these, doesn't compile
        //o.transform_opt([](int&& v){ return optional(v*3);});
        //o.transform_opt([](const int&& v){ return optional(v*3);});
    }

    SECTION("with const lvalue") {
        const optional o(2);
        auto p = o.transform_opt([](const int& v){ return optional(v*2);});
        REQUIRE(p.value() == 4);

        //Can't use these, doesn't compile
        //o.transform_opt([](int& v){ return optional(v*3);});
        //o.transform_opt([](int&& v){ return optional(v*3);});
        //o.transform_opt([](const int&& v){ return optional(v*3);});
    }

    SECTION("with rvalue") {
        auto p = optional(3).transform_opt([](int&& mv) { return optional(mv * 2); });
        REQUIRE(p.value() == 6);

        auto p2 = optional(3).transform_opt([](const int&& mv) { return optional(mv * 3); });
        REQUIRE(p2.value() == 9);

        auto p3 = optional(3).transform_opt([](const int& mv) { return optional(mv * 4); });
        REQUIRE(p3.value() == 12);

        //Can't use these, doesn't compile
        //optional(3).transform_opt([](int& mv) { return optional(mv * 4); });
    }

    SECTION("with const rvalue") {
        auto p = static_cast<const optional<int>&&>(optional(3)).transform_opt([](const int& mv) { return optional(mv * 2); });
        REQUIRE(p.value() == 6);

        //Can't use these, doesn't compile
        //static_cast<const optional<int>&&>(optional(3)).transform_opt([](int& mv) { return optional(mv * 2); });
        //static_cast<const optional<int>&&>(optional(3)).transform_opt([](int&& mv) { return optional(mv * 2); });
        //static_cast<const optional<int>&&>(optional(3)).transform_opt([](const int&& mv) { return optional(mv * 2); });
    }

    SECTION("with no value") {
        optional<int> o;
        auto p = o.transform_opt([](int v){ return optional(v*2);});
        REQUIRE(p.has_value() == false);

        const optional<int> o2;
        auto p2 = o2.transform_opt([](int v){ return optional(v*2);});
        REQUIRE(p2.has_value() == false);

        auto p3 = optional<int>().transform_opt([](int v){ return optional(v*2);});
        REQUIRE(p3.has_value() == false);

        auto p4 = static_cast<const optional<int>&&>(optional<int>()).transform_opt([](int v){ return optional(v*2);});
        REQUIRE(p4.has_value() == false);
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

