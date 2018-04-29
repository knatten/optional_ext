#include "optional_ext.h"
#include "catch.hpp"

#include <string>
#include <iostream>

using std::string;
using knatten::optional;

struct tweet {
    string content;
};

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

TEST_CASE("Demo of call") {
    SECTION("old style") {
        auto foo = find_first("foo");
        if (foo.has_value())
            std::cout << "Found a tweet about 'foo'!";
    }

    SECTION("new style") {
        auto foo = find_first("foo");
        foo.call([](const tweet& t) { std::cout << "Found a tweet about 'foo': " << t.content; });
    }

    SECTION("such oneliner") {
        find_first("foo").call([](const tweet& t) { std::cout << "Found a tweet about 'foo': " << t.content; });
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
