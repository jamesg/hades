#include "catch.hpp"

#include "styx/object.hpp"
#include "styx/styx.hpp"
#include "styx/list.hpp"
#include "styx/object.hpp"

SCENARIO("styx::element") {
    GIVEN("a null element") {
        styx::element e = styx::null_t();
        THEN("styx::is_null reports that the element is null") {
            REQUIRE(styx::is_null(e));
        }
    }
    GIVEN("an object") {
        styx::element e = styx::object();
        THEN("styx::is_null reports that the element is not null") {
            REQUIRE(!styx::is_null(e));
        }
    }
}

