#include "catch.hpp"

#include "hades/compound_id.hpp"

namespace
{
    constexpr char t1_id[] = "t1_id";
    constexpr char t2_id[] = "t2_id";
}

SCENARIO("db::compound_id") {
    GIVEN("a compound id with two keys") {
        hades::compound_id<t1_id, t2_id> id;

        WHEN("the first id is set") {
            id.get_int<t1_id>() = 1;
            THEN("the first id was set") {
                REQUIRE(id.get_int<t1_id>() == 1);
            }
        }

        WHEN("the second id is set") {
            id.get_int<t2_id>() = 2;
            THEN("the second id was set") {
                REQUIRE(id.get_int<t2_id>() == 2);
            }
        }
    }

    GIVEN("a compound id with two keys") {
        hades::compound_id<t1_id, t2_id> id;

        WHEN("the id is constructed with the correct number of elements") {
            id = hades::compound_id<t1_id, t2_id>{1, 2};
            THEN("the first id was set") {
                REQUIRE(id.get_int<t1_id>() == 1);
            }
            THEN("the second id was set") {
                REQUIRE(id.get_int<t2_id>() == 2);
            }
        }
    }
}

