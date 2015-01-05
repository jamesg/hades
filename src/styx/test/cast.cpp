#include "catch.hpp"

#include "styx/cast.hpp"
#include "styx/styx.hpp"

#include "styx/list.hpp"
#include "styx/object.hpp"

SCENARIO("styx::cast") {
    GIVEN("a styx::element string") {
        styx::element e = std::string("51.5");
        WHEN("the element is cast to a double") {
            double d = styx::cast<double>(e);
            THEN("the double was read precisely") {
                REQUIRE(d == 51.5);
            }
        }
    }
}

