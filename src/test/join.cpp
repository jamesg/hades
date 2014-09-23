#include "catch.hpp"

#include "hades/connection.hpp"
#include "hades/join.hpp"

#include "types.hpp"

SCENARIO("hades::join") {
    GIVEN("a new database") {
        hades::connection conn = hades::connection::in_memory_database();
        hades::test::create(conn);

        hades::test::site site;
        site.name() = "Test Site";
        site.insert(conn);

        hades::test::device device_1;
        device_1.site_id() = site.site_id();
        device_1.name() = "Test Device 1";
        device_1.insert(conn);

        hades::test::device device_2;
        device_2.site_id() = site.site_id();
        device_2.name() = "Test Device 2";
        device_2.insert(conn);

        WHEN("devices and sites are joined") {
            styx::list list =
                hades::join<hades::where<>, hades::test::site, hades::test::device>(
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
        }
    }
}

