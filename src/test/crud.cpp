#include "catch.hpp"

#include "hades/connection.hpp"
#include "hades/get_by_id.hpp"
#include "hades/crud.ipp"

#include "types.hpp"

SCENARIO("hades::crud save") {
    GIVEN("a new test database") {
        hades::connection conn = hades::connection::in_memory_database();
        hades::test::create(conn);

        WHEN("a tuple is saved") {
            hades::test::site site;
            site.name() = "Test";
            site.save(conn);

            THEN("the tuple's id attribute has been set") {
                REQUIRE(site.site_id());
            }

            THEN("the tuple can be retrieved") {
                hades::test::site out =
                    hades::get_by_id<hades::test::site>(conn, site.id());
                REQUIRE(out.site_id() == site.site_id());
                REQUIRE(out.name() == site.name());
            }
        }
    }
}

SCENARIO("hades::crud insert") {
    GIVEN("a new test database") {
        hades::connection conn = hades::connection::in_memory_database();
        hades::test::create(conn);

        WHEN("a tuple is inserted") {
            hades::test::site site;
            site.name() = "Test";
            site.insert(conn);

            THEN("the tuple's id attribute has been set") {
                REQUIRE(site.site_id());
            }

            THEN("the tuple can be retrieved") {
                hades::test::site out =
                    hades::get_by_id<hades::test::site>(conn, site.id());
                REQUIRE(out.site_id() == site.site_id());
                REQUIRE(out.name() == site.name());
            }
        }
    }
}

