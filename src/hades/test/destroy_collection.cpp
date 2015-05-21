#include "catch.hpp"

#include "hades/connection.hpp"
#include "hades/crud.ipp"
#include "hades/devoid.hpp"
#include "hades/exists.hpp"

#include "types.hpp"

SCENARIO("hades::crud::destroy_collection") {
    GIVEN("a test database") {
        hades::connection conn(hades::connection::in_memory_database());
        hades::test::create(conn);

        hades::devoid("INSERT INTO SITE(name) VALUES('s1')", conn);
        WHEN("the collection is destroyed") {
            hades::test::site::destroy_collection(conn);
            THEN("the item is no longer present") {
                REQUIRE(!hades::exists<hades::test::site>(conn));
            }
        }

        hades::devoid("INSERT INTO SITE(name) VALUES('s1')", conn);
        hades::devoid("INSERT INTO SITE(name) VALUES('s2')", conn);
        WHEN("the collection is destroyed with a filter") {
            hades::test::site::destroy_collection(
                    hades::where("name = 's2'"),
                    conn
                    );
            THEN("one item remains") {
                REQUIRE(hades::exists<hades::test::site>(conn, hades::where("name = 's1'")));
                REQUIRE(!hades::exists<hades::test::site>(conn, hades::where("name = 's2'")));
            }
        }
    }
}

