#include "catch.hpp"

#include "styx/object_accessor.hpp"

#include "hades/attribute_list.hpp"
#include "hades/crud.hpp"
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
        constexpr char name[] = "name";
    }

    namespace relvar
    {
        constexpr char site[] = "site";
    }

    struct site :
        public hades::crud<site>,
        public hades::relation<relvar::site>,
        public hades::tuple<attribute::site_id, attribute::name>,
        public hades::has_candidate_key<attribute::site_id>
    {
        site()
        {
        }

        int& site_id()
        {
            return get_int<attribute::site_id>();
        }

        std::string& name()
        {
            return get_string<attribute::name>();
        }
    };
}

SCENARIO("hades::insert") {
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

        WHEN("a tuple is inserted") {
            site s;
            s.name() = "new name";
            s.insert(conn);
            THEN("the tuple has been updated") {
                site q_s;
                hades::get_by_id(conn, s.id(), q_s);
                REQUIRE(q_s.name() == s.name());
            }
        }
    }
}

