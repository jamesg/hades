#include "catch.hpp"

#include "styx/object.hpp"

#include "hades/custom_select.hpp"
#include "hades/custom_select_one.hpp"
#include "hades/devoid.hpp"
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

SCENARIO("db::custom_select") {
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
                    "INSERT INTO site(name) VALUES('custom_name')",
                    hades::empty_row(),
                    conn
                    );

            THEN("custom_select_one returns the tuple") {
                site element = hades::custom_select_one<
                    site,
                    hades::row<std::string>,
                    attribute::name>(
                        conn,
                        "SELECT name FROM site WHERE name = ?",
                        hades::row<std::string>("custom_name")
                        );
                REQUIRE(element.name() == "custom_name");
            }
        }
    }
}

