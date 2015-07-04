#include "catch.hpp"

#include "styx/list.hpp"
#include "styx/object.hpp"
#include "styx/object.hpp"

SCENARIO("styx::object::get") {
    GIVEN("an empty styx::object") {
        styx::object o;

        WHEN("the value of a string is obtained using get()") {
            o.get<std::string>("key");
            THEN("the object now contains this key") {
                REQUIRE(o.has_key("key"));
            }
        }

        WHEN("the value of the string is updated") {
            std::string& s = o.get_string("key");
            s = "test";
            THEN("the value was updated in the object") {
                REQUIRE(o.get<std::string>("key") == "test");
            }
            // Note: this would not have worked with 'auto s =
            // o.get_string("key");'.
            THEN("the string cannot be cast to an integer") {
                REQUIRE_THROWS(o.get<styx::int_type>("key"));
            }
        }

        WHEN("the value of the integer is updated") {
            styx::int_type& n = o.get_int("int");
            n = 5;
            THEN("the value was updated in the object") {
                REQUIRE(o.get<styx::int_type>("int") == 5);
            }
            THEN("the integer can be accessed as a string") {
                std::string& s = o.get_string("int");
                REQUIRE(s == "5");
            }
            THEN("the integer can be accessed as a double") {
                double& d = o.get_double("int");
                REQUIRE(d == 5.0);
            }
        }
    }
}

SCENARIO("styx::object::copy") {
    GIVEN("an empty styx::object") {
        styx::object o;

        WHEN("the value of a string is obtained using copy()") {
            o.copy<std::string>("key");
            THEN("the object does not contain this key") {
                REQUIRE(!o.has_key("key"));
            }
        }
    }
}

