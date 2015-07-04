#include "catch.hpp"

#include "styx/object.hpp"

#include "hades/attribute_list.hpp"
#include "hades/devoid.hpp"
#include "hades/get_collection.hpp"
#include "hades/has_candidate_key.hpp"
#include "hades/relation.hpp"
#include "hades/row.hpp"
#include "hades/tuple.hpp"

namespace
{
    namespace attribute
    {
        constexpr char site_id[] = "site_id";
        constexpr char name[] = "name";
    }

    namespace relvar
    {
        constexpr char site[] = "site";
    }

    struct site :
        public hades::relation<relvar::site>,
        public hades::tuple<attribute::site_id, attribute::name>,
        public hades::has_candidate_key<attribute::site_id>
    {
        site()
        {
        }

        site(styx::element& o) :
            styx::object(o)
        {
        }

        styx::int_type& site_id()
        {
            return key<attribute::site_id>();
        }

        std::string& name()
        {
            return get_string<attribute::name>();
        }
    };
}

SCENARIO("db::get_collection") {
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

            THEN("get_collection returns the tuple") {
                styx::list list = hades::get_collection<site>(conn);
                REQUIRE(list.size() == 1);
                REQUIRE(site(list[0]).site_id() == 1);
                REQUIRE(site(list[0]).name() == "name_value");
            }
        }
    }
}

