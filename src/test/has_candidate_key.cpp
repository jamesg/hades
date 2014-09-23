#include "catch.hpp"

#include "styx/object_accessor.hpp"

#include "hades/has_candidate_key.hpp"

namespace
{
    constexpr char device_id_cstr[] = "device_id";
    constexpr char site_id_cstr[] = "site_id";

    struct device :
        public hades::has_candidate_key<device_id_cstr, site_id_cstr>
    {
        device(styx::element& o) :
            styx::object_accessor(o)
        {
        }
    };
}

SCENARIO("device id") {
    GIVEN("a new device") {
        styx::element o;
        device d(o);
        WHEN("the device id is set") {
            d.key<device_id_cstr>() = 1;
            THEN("the device id can be read") {
                REQUIRE(d.key<device_id_cstr>() == 1);
            }
        }
        WHEN("the site id is set") {
            d.key<site_id_cstr>() = 1;
            THEN("the site is can be read") {
                REQUIRE(d.key<site_id_cstr>() == 1);
            }
        }
        WHEN("both ids are set") {
            d.key<device_id_cstr>() = 1;
            d.key<site_id_cstr>() = 2;
            THEN("the ids are different") {
                REQUIRE(d.key<device_id_cstr>() == 1);
                REQUIRE(d.key<site_id_cstr>() == 2);
            }
        }
    }
}

