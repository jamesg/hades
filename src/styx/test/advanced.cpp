#include "catch.hpp"

#include "styx/list.hpp"
#include "styx/object.hpp"
#include "styx/styx.hpp"
#include "styx/object.hpp"

SCENARIO("advanced parser tests", "[parser]") {
    /*
     * The parser should be able to remove leading and training whitespace from
     * the elements it parses.
     */

    GIVEN("an empty list with whitespace") {
        const std::string empty_list = "  [ \n ]  \n ";
        WHEN("the list is parsed") {
            styx::element o = styx::parse_json(empty_list);
            THEN("the list is empty") {
                REQUIRE(boost::get<styx::list>(o).size() == 0);
            }
        }
    }
    GIVEN("an empty list with no whitespace") {
        const std::string empty_list = "[]";
        WHEN("the list is parsed") {
            styx::element o = styx::parse_json(empty_list);
            THEN("the list is empty") {
                REQUIRE(boost::get<styx::list>(o).size() == 0);
            }
        }
    }
}

