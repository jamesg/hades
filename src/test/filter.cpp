#include "catch.hpp"

#include "hades/connection.hpp"
#include "hades/crud.ipp"
#include "hades/filter.hpp"
#include "hades/get_one.hpp"

#include "types.hpp"

SCENARIO("hades::filter") {
    GIVEN("a new database") {
        hades::connection conn = hades::connection::in_memory_database();
        hades::test::create(conn);

        hades::test::site s1;
        s1.name() = "Test 1";
        s1.insert(conn);
        hades::test::site s2;
        s2.name() = "Test 2";
        s2.insert(conn);

        WHEN("a query is made for a site by name") {
            auto where = hades::where(
                    "name = ?",
                    hades::row<std::string>(s1.name())
                    );
            styx::list list = hades::test::site::get_collection(conn, where);
            THEN("site 1 is returned") {
                REQUIRE(list.size() == 1);
                hades::test::site s1_out(list.at(0));
                REQUIRE(s1_out.name() == s1.name());
            }
        }
    }
}

SCENARIO("hades::filter::and_") {
    GIVEN("a new database") {
        hades::connection conn = hades::connection::in_memory_database();
        hades::test::create(conn);

        hades::test::site s1;
        s1.name() = "Test 1";
        s1.insert(conn);
        hades::test::site s2;
        s2.name() = "Test 2";
        s2.insert(conn);

        WHEN("a query with a where clause on two attributes is made") {
            auto where = hades::and_(
                    hades::where(
                        "name = ?",
                        hades::row<std::string>(s1.name())
                        ),
                    hades::where(
                        "site_id = ?",
                        hades::row<int>(s1.site_id())
                        )
                    );
            THEN("site 1 is returned") {
                hades::test::site s1_out =
                    hades::get_one<hades::test::site>(conn, where);
                REQUIRE(s1_out.name() == s1.name());
            }
        }
    }
}

