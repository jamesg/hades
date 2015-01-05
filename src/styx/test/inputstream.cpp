#include <sstream>
#include <stdlib.h>

#include "catch.hpp"

SCENARIO("std::istringstream") {
    GIVEN("a std::istringstream") {
        std::istringstream is("0.5");
        WHEN("the stream is read into a double") {
            double d;
            is >> d;
            THEN("the double was read precisely") {
                REQUIRE(d == 0.5);
            }
        }
    }
    GIVEN("a string") {
        std::string s = "51.5";
        WHEN("the string is converted to a double using strtod") {
            double d = strtod(s.c_str(), nullptr);
            THEN("the double was read precisely") {
                REQUIRE(d == 51.5);
            }
        }
    }
}

