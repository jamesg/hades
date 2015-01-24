#include "catch.hpp"

#include "styx/object.hpp"

#include "hades/attribute_list.hpp"
#include "hades/devoid.hpp"
#include "hades/get_one.hpp"
#include "hades/has_candidate_key.hpp"
#include "hades/relation.hpp"
#include "hades/row.hpp"
#include "hades/tuple.hpp"
#include "types.hpp"

SCENARIO("db::get_one") {
    GIVEN("a test database") {
        hades::connection conn = hades::connection::in_memory_database();
        hades::test::create(conn);

        WHEN("a site is inserted") {
            hades::test::site site;
            site.get_string<hades::test::attr::name>() = "Test";
            site.insert(conn);

            THEN("the site can be retrieved by name") {
                auto where = hades::where(
                        "site.name = ?",
                        hades::row<std::string>(
                            site.get_string<hades::test::attr::name>()
                            )
                        );
                hades::test::site site_out =
                    hades::get_one<hades::test::site>(conn, where);
                REQUIRE(
                    site_out.get_int<hades::test::attr::site_id>() ==
                    site.get_int<hades::test::attr::site_id>()
                    );
                REQUIRE(
                    site_out.get_string<hades::test::attr::name>() ==
                    site.get_string<hades::test::attr::name>()
                    );
            }
        }
    }
}

