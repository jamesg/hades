#include "catch.hpp"

#include "styx/object.hpp"

#include "hades/attribute_list.hpp"
#include "hades/devoid.hpp"
#include "hades/get_by_id.hpp"
#include "hades/has_candidate_key.hpp"
#include "hades/relation.hpp"
#include "hades/row.hpp"
#include "hades/tuple.hpp"

namespace
{
    namespace attribute
    {
        constexpr char site_id[] = "site_id";
        constexpr char device_id[] = "device_id";
        constexpr char relay_id[] = "relay_id";
        constexpr char name[] = "name";
    }

    namespace relvar
    {
        constexpr char site[] = "site";
        constexpr char device[] = "device";
        constexpr char relay[] = "relay";
    }

    struct site :
        public hades::relation<relvar::site>,
        public hades::tuple<attribute::site_id, attribute::name>,
        public hades::has_candidate_key<attribute::site_id>
    {
        site()
        {
        }
        site(const styx::element& o) :
            styx::object(o)
        {
        }
        int& site_id()
        {
            return key<attribute::site_id>();
        }
        std::string& name()
        {
            return get_string<attribute::name>();
        }
    };

    struct device :
        public hades::relation<relvar::device>,
        public hades::tuple<attribute::site_id, attribute::device_id, attribute::name>,
        public hades::has_candidate_key<attribute::site_id, attribute::device_id>
    {
        device()
        {
        }
        device(const styx::element& o) :
            styx::object(o)
        {
        }
        int& site_id()
        {
            return key<attribute::site_id>();
        }
        int& device_id()
        {
            return key<attribute::device_id>();
        }
        std::string& name()
        {
            return get_string<attribute::name>();
        }
    };
}

SCENARIO("db::get_by_id") {
    GIVEN("a single id relation (site)") {
        hades::connection conn = hades::connection::in_memory_database();

        hades::devoid(
                "CREATE TABLE site ( "
                " site_id INTEGER PRIMARY KEY AUTOINCREMENT, "
                " name VARCHAR DEFAULT 'name_value' "
                " )",
                hades::empty_row(),
                conn
                );

        WHEN("one tuple is inserted") {
            hades::devoid(
                    "INSERT INTO site DEFAULT VALUES",
                    hades::empty_row(),
                    conn
                    );

            THEN("get_by_id returns the tuple") {
                site s = hades::get_by_id<site>(conn, site::id_type{1});
                REQUIRE(s.site_id() == 1);
            }
        }
    }

    GIVEN("a dual id relation (device)") {
        hades::compound_id<attribute::site_id> id{1};

        hades::connection conn = hades::connection::in_memory_database();

        hades::devoid(
                "CREATE TABLE device ( "
                " site_id INTEGER, "
                " device_id INTEGER, "
                " name VARCHAR DEFAULT 'name_value', "
                " PRIMARY KEY (site_id, device_id) "
                " )",
                hades::empty_row(),
                conn
                );

        WHEN("one tuple is inserted") {
            hades::devoid(
                    "INSERT INTO device(site_id, device_id, name) "
                    " VALUES (1, 1, 'name') ",
                    hades::empty_row(),
                    conn
                    );

            THEN("get_by_id returns the tuple") {
                device d =
                    hades::get_by_id<device>(conn, device::id_type{1, 1});
                REQUIRE(d.site_id() == 1);
                REQUIRE(d.device_id() == 1);
                REQUIRE(d.name() == "name");
            }
        }

        WHEN("two tuples are inserted") {
            hades::devoid(
                    "INSERT INTO device(site_id, device_id, name) "
                    " VALUES (1, 1, 'name1') ",
                    hades::empty_row(),
                    conn
                    );

            hades::devoid(
                    "INSERT INTO device(site_id, device_id, name) "
                    " VALUES (2, 2, 'name2') ",
                    hades::empty_row(),
                    conn
                    );

            THEN("get_by_id returns the tuple") {
                device d =
                    hades::get_by_id<device>(conn, device::id_type{2, 2});
                REQUIRE(d.site_id() == 2);
                REQUIRE(d.device_id() == 2);
                REQUIRE(d.name() == "name2");
            }
        }
    }
}

