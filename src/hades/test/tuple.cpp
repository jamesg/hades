#include "catch.hpp"

#include "styx/list.hpp"
#include "styx/object.hpp"
#include "hades/tuple.hpp"

namespace
{
    constexpr char t1_id[] = "t1_id";
    constexpr char t2_id[] = "t2_id";

    // Example for writing classes deriving from hades::tuple.
    class test_tuple_type : public hades::tuple<t1_id, t2_id>
    {
    public:
        // Default constructor only needs to be declared if another constructor
        // is declared.
        test_tuple_type()
        {
        }
        // Define a constructor like this to enable construction from a
        // styx::element.  This is useful for serving API requests that receive
        // an element of unknown type.
        test_tuple_type(const styx::element& o) :
            styx::object(o)
        {
        }
    };
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

SCENARIO("hades::tuple assignment") {
    GIVEN("a tuple") {
        hades::tuple<t1_id, t2_id> tuple;
        tuple.get_string<t1_id>() = "1";
        tuple.get_string<t2_id>() = "2";

        WHEN("the tuple is copy constructed") {
            hades::tuple<t1_id, t2_id> tuple2(tuple);
            THEN("the attributes were copied") {
                REQUIRE(tuple2.get_string<t1_id>() == "1");
                REQUIRE(tuple2.get_string<t2_id>() == "2");
            }
        }
    }
    GIVEN("a tuple with all attributes set") {
        test_tuple_type tuple;
        tuple.get_string<t1_id>() = "1";
        tuple.get_string<t2_id>() = "2";
        WHEN("the tuple is assigned") {
            test_tuple_type tuple2;
            tuple2.get_string<t1_id>() = "3";
            tuple2 = tuple;
            THEN("all attributes were set") {
                REQUIRE(tuple2.get_string<t1_id>() == "1");
                REQUIRE(tuple2.get_string<t2_id>() == "2");
            }
        }
    }
}

