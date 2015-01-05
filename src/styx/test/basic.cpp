#include "catch.hpp"

#include "styx/list.hpp"
#include "styx/object.hpp"
#include "styx/styx.hpp"
//#include "styx/object.hpp"

SCENARIO("basic parser tests", "[parser]") {
    GIVEN("a boolean") {
        const std::string bool_true = "true";
        WHEN("the bool is parsed") {
            styx::element o = styx::parse_json(bool_true);
            THEN("the boolean can be read") {
                REQUIRE(boost::get<bool>(o) == true);
            }
        }
    }
    GIVEN("a basic object") {
        const std::string basic_element = "{ \"a\": \"b\" }";
        WHEN("the element is parsed") {
            styx::element o = styx::parse_json(basic_element);
            THEN("We can access attribute a") {
                REQUIRE(styx::object(o).get_string("a") == "b");
            }
        }
    }
    GIVEN("An empty element") {
        const std::string empty_element = "{ }";
        WHEN("the element is parsed") {
            styx::element o = styx::parse_json(empty_element);
            THEN("the element was parsed successfully but is empty") {
                REQUIRE(styx::object(o).m_map.size() == 0);
            }
        }
    }
    GIVEN("A basic list") {
        const std::string basic_list = "[ \"a\", \"b\" ]";
        WHEN("the list is parsed") {
            styx::element o = styx::parse_json(basic_list);
            THEN("the list contains members \"a\" and \"b\"") {
                REQUIRE(boost::get<styx::list>(o).size() == 2);
                REQUIRE(boost::get<styx::list>(o).get<std::string>(0) == "a");
                REQUIRE(boost::get<styx::list>(o).get<std::string>(1) == "b");
            }
        }
    }
    GIVEN("An empty list") {
        const std::string empty_list = "[ ]";
        WHEN("the list is parsed") {
            styx::element o = styx::parse_json(empty_list);
            THEN("the list is empty") {
                REQUIRE(boost::get<styx::list>(o).size() == 0);
            }
        }
    }
    GIVEN("An object with a integer") {
        const std::string basic_element = "{ \"a\": 1 }";
        WHEN("the element is parsed") {
            styx::element o = styx::parse_json(basic_element);
            THEN("We can access attribute a") {
                REQUIRE(styx::object(o).get_int("a") == 1);
            }
        }
    }
    GIVEN("An object with a double") {
        const std::string basic_element = "{ \"a\": 0.1 }";
        WHEN("the element is parsed") {
            styx::element o = styx::parse_json(basic_element);
            THEN("We can access attribute a") {
                float a = styx::object(o).get_double("a");
                bool close = a/0.1 < 1.1 && a/0.1 > 0.9;
                REQUIRE(close);
            }
        }
    }
    GIVEN("an object with a list") {
        const std::string s = "{ \"a\": [ \"b\" ] }";
        WHEN("the element is parsed") {
            styx::element o = styx::parse_json(s);
            THEN("attribute a is a list containing string b") {
                REQUIRE(styx::object(o).get_list("a").size() == 1);
                REQUIRE(
                        boost::get<std::string>(
                            styx::object(o).get_list("a").at(0)
                            )
                        == "b"
                       );
            }
        }
    }
    GIVEN("a list with an object") {
        const std::string s = "[ { \"a\": \"b\" } ]";
        WHEN("the element is parsed") {
            THEN("the object was parsed correctly") {
                styx::element o = styx::parse_json(s);
                REQUIRE(boost::get<styx::list>(o).size() == 1);
                REQUIRE(
                        styx::object(boost::get<styx::list>(o).at(0))
                            .get_string("a") == "b"
                        );
            }
        }
    }
    GIVEN("a list with two objects") {
        const std::string s = "[ { \"a\": \"b\" }, { \"c\": \"d\" } ]";
        WHEN("the element is parsed") {
            THEN("the object was parsed correctly") {
                styx::element o = styx::parse_json(s);
                REQUIRE(boost::get<styx::list>(o).size() == 2);
                REQUIRE(
                        styx::object(boost::get<styx::list>(o).at(0))
                            .get_string("a") == "b"
                        );
                REQUIRE(
                        styx::object(boost::get<styx::list>(o).at(1))
                            .get_string("c") == "d"
                        );
            }
        }
    }
    GIVEN("a very long list") {
        const std::string s =
            "["
            // 0
            "\"a\", \"a\", \"a\", \"a\", \"a\", "
            "\"a\", \"a\", \"a\", \"a\", \"a\", "
            "\"a\", \"a\", \"a\", \"a\", \"a\", "
            "\"a\", \"a\", \"a\", \"a\", \"a\", "
            "\"a\", \"a\", \"a\", \"a\", \"a\", "
            "\"a\", \"a\", \"a\", \"a\", \"a\", "
            "\"a\", \"a\", \"a\", \"a\", \"a\", "
            "\"a\", \"a\", \"a\", \"a\", \"a\", "
            "\"a\", \"a\", \"a\", \"a\", \"a\", "
            "\"a\", \"a\", \"a\", \"a\", \"a\", "
            "\"a\", \"a\", \"a\", \"a\", \"a\", "
            "\"a\", \"a\", \"a\", \"a\", \"a\", "
            "\"a\", \"a\", \"a\", \"a\", \"a\", "
            "\"a\", \"a\", \"a\", \"a\", \"a\", "
            "\"a\", \"a\", \"a\", \"a\", \"a\", "
            "\"a\", \"a\", \"a\", \"a\", \"a\", "
            "\"a\", \"a\", \"a\", \"a\", \"a\", "
            "\"a\", \"a\", \"a\", \"a\", \"a\", "
            "\"a\", \"a\", \"a\", \"a\", \"a\", "
            "\"a\", \"a\", \"a\", \"a\", \"a\", "
            // 100
            "\"a\", \"a\", \"a\", \"a\", \"a\", "
            "\"a\", \"a\", \"a\", \"a\", \"a\", "
            "\"a\", \"a\", \"a\", \"a\", \"a\", "
            "\"a\", \"a\", \"a\", \"a\", \"a\", "
            "\"a\", \"a\", \"a\", \"a\", \"a\", "
            "\"a\", \"a\", \"a\", \"a\", \"a\", "
            "\"a\", \"a\", \"a\", \"a\", \"a\", "
            "\"a\", \"a\", \"a\", \"a\", \"a\", "
            "\"a\", \"a\", \"a\", \"a\", \"a\", "
            "\"a\", \"a\", \"a\", \"a\", \"a\", "
            "\"a\", \"a\", \"a\", \"a\", \"a\", "
            "\"a\", \"a\", \"a\", \"a\", \"a\", "
            "\"a\", \"a\", \"a\", \"a\", \"a\", "
            "\"a\", \"a\", \"a\", \"a\", \"a\", "
            "\"a\", \"a\", \"a\", \"a\", \"a\", "
            "\"a\", \"a\", \"a\", \"a\", \"a\", "
            "\"a\", \"a\", \"a\", \"a\", \"a\", "
            "\"a\", \"a\", \"a\", \"a\", \"a\", "
            "\"a\", \"a\", \"a\", \"a\", \"a\", "
            "\"a\", \"a\", \"a\", \"a\", \"a\", "
            // 200
            "\"a\", \"a\", \"a\", \"a\", \"a\", "
            "\"a\", \"a\", \"a\", \"a\", \"a\", "
            "\"a\", \"a\", \"a\", \"a\", \"a\", "
            "\"a\", \"a\", \"a\", \"a\", \"a\", "
            "\"a\", \"a\", \"a\", \"a\", \"a\", "
            "\"a\", \"a\", \"a\", \"a\", \"a\", "
            "\"a\", \"a\", \"a\", \"a\", \"a\", "
            "\"a\", \"a\", \"a\", \"a\", \"a\", "
            "\"a\", \"a\", \"a\", \"a\", \"a\", "
            "\"a\", \"a\", \"a\", \"a\", \"a\", "
            "\"a\", \"a\", \"a\", \"a\", \"a\", "
            "\"a\", \"a\", \"a\", \"a\", \"a\", "
            "\"a\", \"a\", \"a\", \"a\", \"a\", "
            "\"a\", \"a\", \"a\", \"a\", \"a\", "
            "\"a\", \"a\", \"a\", \"a\", \"a\", "
            "\"a\", \"a\", \"a\", \"a\", \"a\", "
            "\"a\", \"a\", \"a\", \"a\", \"a\", "
            "\"a\", \"a\", \"a\", \"a\", \"a\", "
            "\"a\", \"a\", \"a\", \"a\", \"a\", "
            "\"a\", \"a\", \"a\", \"a\", \"a\", "
            // 300
            "\"a\", \"a\", \"a\", \"a\", \"a\", "
            "\"a\", \"a\", \"a\", \"a\", \"a\", "
            "\"a\", \"a\", \"a\", \"a\", \"a\", "
            "\"a\", \"a\", \"a\", \"a\", \"a\", "
            "\"a\", \"a\", \"a\", \"a\", \"a\", "
            "\"a\", \"a\", \"a\", \"a\", \"a\", "
            "\"a\", \"a\", \"a\", \"a\", \"a\", "
            "\"a\", \"a\", \"a\", \"a\", \"a\", "
            "\"a\", \"a\", \"a\", \"a\", \"a\", "
            "\"a\", \"a\", \"a\", \"a\", \"a\", "
            "\"a\", \"a\", \"a\", \"a\", \"a\", "
            "\"a\", \"a\", \"a\", \"a\", \"a\", "
            "\"a\", \"a\", \"a\", \"a\", \"a\", "
            "\"a\", \"a\", \"a\", \"a\", \"a\", "
            "\"a\", \"a\", \"a\", \"a\", \"a\", "
            "\"a\", \"a\", \"a\", \"a\", \"a\", "
            "\"a\", \"a\", \"a\", \"a\", \"a\", "
            "\"a\", \"a\", \"a\", \"a\", \"a\", "
            "\"a\", \"a\", \"a\", \"a\", \"a\", "
            "\"a\", \"a\", \"a\", \"a\", \"a\" "
            // 400
            " ]"
            ;
        WHEN("the element is parsed") {
            THEN("the object was parsed correctly") {
                styx::element o = styx::parse_json(s);
                REQUIRE(boost::get<styx::list>(o).size() == 400);
                REQUIRE(
                        boost::get<std::string>(
                            boost::get<styx::list>(o).at(0)
                            ) == "a"
                        );
            }
        }
    }
}

