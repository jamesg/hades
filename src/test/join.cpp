#include "catch.hpp"

#include "hades/connection.hpp"
#include "hades/crud.ipp"
#include "hades/join.hpp"

#include "types.hpp"

SCENARIO("hades::join") {
    GIVEN("a new database") {
        hades::connection conn = hades::connection::in_memory_database();
        hades::test::create(conn);

        hades::test::site site_1;
        site_1.name() = "Test Site 1";
        site_1.insert(conn);

        hades::test::site site_2;
        site_2.name() = "Test Site 2";
        site_2.get_bool<hades::test::flag::site::enabled>() = true;
        site_2.save(conn);

        hades::test::device device_1;
        device_1.site_id() = site_1.site_id();
        device_1.name() = "Test Device 1";
        device_1.insert(conn);

        hades::test::device device_2;
        device_2.site_id() = site_1.site_id();
        device_2.name() = "Test Device 2";
        device_2.insert(conn);

        //hades::test::site_enabled enabled(site_2.id());
        //enabled.save(conn);

        WHEN("a join on one type is requested") {
            styx::list list = hades::equi_join<hades::test::site>(conn);
            THEN("the join is equivalent to get_collection") {
            }
        }

        WHEN("device is joined with device_enabled") {
            styx::list list =
                hades::equi_join<hades::test::site, hades::test::site_enabled>(
                    conn
                    );
            THEN("both sites were returned") {
                REQUIRE(list.size() == 2);
            }
            THEN("the first site is equal to the input and is not enabled") {
                hades::test::site site_1_out(list[0]);
                REQUIRE(site_1_out.name() == site_1.name());
                bool test = site_1_out.get_bool<hades::test::flag::site::enabled>();
                REQUIRE(!test);
            }
            THEN("the second site is equal to the input and is enabled") {
                hades::test::site site_2_out(list[1]);
                REQUIRE(site_2_out.name() == site_2.name());
                bool test = site_2_out.get_bool<hades::test::flag::site::enabled>();
                REQUIRE(test);
            }
        }

        WHEN("devices and sites are joined") {
            styx::list list =
                hades::join<hades::test::site, hades::test::device>(
                    conn,
                    hades::where<>("site.site_id = device.site_id")
                    );
            THEN("two devices were returned") {
                REQUIRE(list.size() == 2);
            }
            THEN("the first device is equal to the input") {
                hades::test::device device_1_out(list[0]);
                REQUIRE(device_1_out.name() == device_1.name());
            }
            THEN("the second device is equal to the input") {
                hades::test::device device_2_out(list[1]);
                REQUIRE(device_2_out.name() == device_2.name());
            }
        }
    }
}

