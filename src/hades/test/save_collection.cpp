#include "catch.hpp"

#include "hades/connection.hpp"
#include "hades/crud.ipp"
#include "hades/exists.hpp"

#include "types.hpp"

SCENARIO("hades::crud::save_collection") {
    GIVEN("an empty test database") {
        hades::connection conn(hades::connection::in_memory_database());
        hades::test::create(conn);

        WHEN("collections with one and two items are saved") {
            {
                styx::list l;
                hades::test::site s1;
                s1.get_string<hades::test::attr::name>() = "s1";
                l.append(s1);
                hades::test::site::save_collection(l, conn);
            }
            THEN("the collection was saved") {
                REQUIRE(hades::exists<hades::test::site>(conn, hades::where("name = 's1'")));
            }

            styx::list l;

            hades::test::site s2;
            s2.get_string<hades::test::attr::name>() = "s2";
            l.append(s2);

            hades::test::site s3;
            s3.get_string<hades::test::attr::name>() = "s3";
            l.append(s3);

            hades::test::site::save_collection(l, conn);

            THEN("the original collection remains") {
                REQUIRE(hades::exists<hades::test::site>(conn, hades::where("name = 's1'")));
            }

            THEN("the new collection was saved") {
                REQUIRE(hades::exists<hades::test::site>(conn, hades::where("name = 's2'")));
                REQUIRE(hades::exists<hades::test::site>(conn, hades::where("name = 's3'")));
            }
        }

        WHEN("an existing item is modified") {
            styx::list l;
            hades::test::site s1;
            s1.get_string<hades::test::attr::name>() = "s1";
            s1.save(conn);

            s1.get_string<hades::test::attr::name>() = "s2";
            l.append(s1);
            hades::test::site::save_collection(l, conn);

            THEN("only the updated item is present") {
                REQUIRE(!hades::exists<hades::test::site>(conn, hades::where("name = 's1'")));
                REQUIRE(hades::exists<hades::test::site>(conn, hades::where("name = 's2'")));
            }
        }
    }
}

