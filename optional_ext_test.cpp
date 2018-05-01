#include "optional_ext.h"
#include "supporting_types.h"
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
        auto p2 = static_cast<const optional<int>&&>(optional<int>(2)).transform([](const int&& mv){ return mv * 3;});
        REQUIRE(p2.value() == 6);

        //Can't use these, doesn't compile
        //static_cast<const optional<int>&&>(optional<int>(2)).transform([](int& mv){ return mv * 2;});
        //static_cast<const optional<int>&&>(optional<int>(2)).transform([](int&& mv){ return mv * 2;});
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

TEST_CASE("transform_optional") {
    SECTION("with lvalue") {
        optional o(2);
        auto p = o.transform_optional([](int& v){ return optional(v*2);});
        REQUIRE(p.value() == 4);

        auto p2 = o.transform_optional([](const int& v){ return optional(v*3);});
        REQUIRE(p2.value() == 6);

        //Can't use these, doesn't compile
        //o.transform_optional([](int&& v){ return optional(v*3);});
        //o.transform_optional([](const int&& v){ return optional(v*3);});
    }

    SECTION("with const lvalue") {
        const optional o(2);
        auto p = o.transform_optional([](const int& v){ return optional(v*2);});
        REQUIRE(p.value() == 4);

        //Can't use these, doesn't compile
        //o.transform_optional([](int& v){ return optional(v*3);});
        //o.transform_optional([](int&& v){ return optional(v*3);});
        //o.transform_optional([](const int&& v){ return optional(v*3);});
    }

    SECTION("with rvalue") {
        auto p = optional(3).transform_optional([](int&& mv) { return optional(mv * 2); });
        REQUIRE(p.value() == 6);

        auto p2 = optional(3).transform_optional([](const int&& mv) { return optional(mv * 3); });
        REQUIRE(p2.value() == 9);

        auto p3 = optional(3).transform_optional([](const int& mv) { return optional(mv * 4); });
        REQUIRE(p3.value() == 12);

        //Can't use these, doesn't compile
        //optional(3).transform_optional([](int& mv) { return optional(mv * 4); });
    }

    SECTION("with const rvalue") {
        auto p = static_cast<const optional<int>&&>(optional(3)).transform_optional([](const int& mv) { return optional(mv * 2); });
        REQUIRE(p.value() == 6);
        auto p2 = static_cast<const optional<int>&&>(optional(3)).transform_optional([](const int&& mv) { return optional(mv * 3); });
        REQUIRE(p2.value() == 9);

        //Can't use these, doesn't compile
        //static_cast<const optional<int>&&>(optional(3)).transform_optional([](int& mv) { return optional(mv * 2); });
        //static_cast<const optional<int>&&>(optional(3)).transform_optional([](int&& mv) { return optional(mv * 2); });
    }

    SECTION("with no value") {
        optional<int> o;
        auto p = o.transform_optional([](int v){ return optional(v*2);});
        REQUIRE(p.has_value() == false);

        const optional<int> o2;
        auto p2 = o2.transform_optional([](int v){ return optional(v*2);});
        REQUIRE(p2.has_value() == false);

        auto p3 = optional<int>().transform_optional([](int v){ return optional(v*2);});
        REQUIRE(p3.has_value() == false);

        auto p4 = static_cast<const optional<int>&&>(optional<int>()).transform_optional([](int v){ return optional(v*2);});
        REQUIRE(p4.has_value() == false);
    }
}

TEST_CASE("call") {
    SECTION("with lvalue") {
        auto o = optional(2);
        bool called_non_const = false;
        o.call([&called_non_const](int&){ called_non_const = true; });
        REQUIRE(called_non_const == true);

        bool called_const = false;
        o.call([&called_const](const int&){ called_const = true; });
        REQUIRE(called_const == true);

        //Can't use these, doesn't compile
        //o.call([](int&&){});
        //o.call([](const int&&){});
    }
    SECTION("with const lvalue") {
        const auto o = optional(2);
        bool called_const = false;
        o.call([&called_const](const int&){ called_const = true; });
        REQUIRE(called_const == true);

        //Can't use these, doesn't compile
        //o.call([](int&){});
        //o.call([](int&&){});
        //o.call([](const int&&){});
    }
    SECTION("with rvalue") {
        bool called_rvalue = false;
        optional<int>(2).call([&called_rvalue](int&&){ called_rvalue = true; });
        REQUIRE(called_rvalue == true);

        bool called_const_rvalue = false;
        optional<int>(2).call([&called_const_rvalue](const int&&){ called_const_rvalue = true; });
        REQUIRE(called_const_rvalue == true);

        bool called_const_lvalue = false;
        optional<int>(2).call([&called_const_lvalue](int&&){ called_const_lvalue = true; });
        REQUIRE(called_const_lvalue == true);

        //Can't use these, doesn't compile
        //optional<int>(2).call([](int&){});
    }
    SECTION("with const rvalue") {
        bool called_const_rvalue = false;
        static_cast<const optional<int>&&>(optional<int>(2)).call([&called_const_rvalue](const int&&){ called_const_rvalue = true; });
        REQUIRE(called_const_rvalue == true);

        bool called_const_lvalue = false;
        static_cast<const optional<int>&&>(optional<int>(2)).call([&called_const_lvalue](const int&){ called_const_lvalue = true; });
        REQUIRE(called_const_lvalue == true);

        //Can't use these, doesn't compile
        //static_cast<const optional<int>&&>(optional<int>(2)).call([](int&&){});
        //static_cast<const optional<int>&&>(optional<int>(2)).call([](int&){});
    }
    SECTION("with no value") {
        bool called1 = false;
        optional<int> o1;
        o1.call([&called1](int){ called1=true;});
        REQUIRE(called1 == false);

        bool called2 = false;
        const optional<int> o2;
        o2.call([&called2](int){ called2=true;});
        REQUIRE(called2 == false);

        bool called3 = false;
        optional<int>().call([&called3](int){ called3=true;});
        REQUIRE(called3 == false);

        bool called4 = false;
        static_cast<optional<int>>(optional<int>()).call([&called4](int){ called4=true;});
        REQUIRE(called4 == false);
    }
}

using std::cout, std::endl;
TEST_CASE("deleteme") {
    unmovable a;
    unmovable b(a);
    unmovable c = a;
    unmovable d;
    d = a;
    /* unmovable e(std::move(d)); */
    /* c = std::move(b); */
}

TEST_CASE("deleteme2") {
    uncopyable a;
    /* uncopyable b(a); */
    /* uncopyable c = a; */
    /* uncopyable d; */
    /* d = a; */
    uncopyable e(std::move(a));
    uncopyable f;
    f = std::move(e);
}
