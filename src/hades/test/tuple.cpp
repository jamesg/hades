#include "catch.hpp"

#include "styx/list.hpp"
#include "styx/object.hpp"
#include "hades/tuple.hpp"

namespace
{
    constexpr char t1_id[] = "t1_id";
    constexpr char t2_id[] = "t2_id";
}

SCENARIO("hades::tuple::to_row") {
    GIVEN("a tuple with all attributes set") {
        hades::tuple<t1_id, t2_id> tuple;
        tuple.get_string<t1_id>() = "1";
        tuple.get_string<t2_id>() = "2";

        WHEN("the tuple is converted to a row") {
            auto r = tuple.to_string_row();
            THEN("both attributes were set") {
                auto c1 = hades::column<0, decltype(r)>(r);
                REQUIRE(c1 == "1");
                auto c2 = hades::column<1, decltype(r)>(r);
                REQUIRE(c2 == "2");
            }
        }
    }

    GIVEN("a tuple with one attribute set and one attribute not set") {
        hades::tuple<t1_id, t2_id> tuple;
        tuple.get_string<t1_id>() = "1";

        WHEN("the tuple is converted to a row") {
            auto r = tuple.to_string_row();
            THEN("both attributes were set") {
                auto c1 = hades::column<0, decltype(r)>(r);
                REQUIRE(c1 == "1");
                auto c2 = hades::column<1, decltype(r)>(r);
                REQUIRE(c2 == "");
            }
        }
    }
}

