#include "catch.hpp"

#include "styx/object_accessor.hpp"

#include "hades/attribute_list.hpp"
#include "hades/crud.hpp"
#include "hades/crud.ipp"
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
        site(styx::element& o) :
            styx::object_accessor(o)
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
}

SCENARIO("hades::update") {
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
        hades::devoid(
                "INSERT INTO site DEFAULT VALUES",
                hades::empty_row(),
                conn
                );

        WHEN("a tuple is updated") {
            styx::element s_o;
            site s(s_o);
            s.site_id() = 1;
            s.name() = "new name";
            s.update(conn);
            THEN("the tuple has been updated") {
                styx::element q_s_o;
                site q_s(q_s_o);
                hades::get_by_id(conn, site::id_type{1}, q_s);
                REQUIRE(q_s.name() == s.name());
            }
        }
    }
}

