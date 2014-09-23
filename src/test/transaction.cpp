#include "catch.hpp"

#include "hades/connection.hpp"
#include "hades/custom_select.hpp"
#include "hades/devoid.hpp"
#include "hades/has_candidate_key.hpp"
#include "hades/relation.hpp"
#include "hades/row.hpp"
#include "hades/transaction.hpp"
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

SCENARIO("hades::transaction") {
    GIVEN("a simple database") {
        hades::connection conn = hades::connection::in_memory_database();
        hades::devoid(
                "CREATE TABLE site ( "
                " site_id INTEGER PRIMARY KEY AUTOINCREMENT, "
                " name VARCHAR "
                " ) ",
                hades::empty_row(),
                conn
                );
        WHEN("one transaction is run but not committed") {
            {
                hades::transaction tr(conn, "one_transaction_test");
                hades::devoid("INSERT INTO site(name) VALUES('test')", conn);
                styx::list l = hades::custom_select<site>(
                    conn,
                    "SELECT 1 FROM site "
                    );
                REQUIRE(l.size() == 1);
            }
            THEN("the transaction had no effect") {
                styx::list l = hades::custom_select<site>(
                    conn,
                    "SELECT 1 FROM site "
                    );
                REQUIRE(l.size() == 0);
            }
        }
        WHEN("one transaction is run and committed") {
            {
                hades::transaction tr(conn, "one_transaction_test");
                hades::devoid("INSERT INTO site(name) VALUES('test')", conn);
                tr.commit();
            }
            THEN("the transaction inserted a row") {
                styx::list l = hades::custom_select<site>(
                    conn,
                    "SELECT 1 FROM site "
                    );
                REQUIRE(l.size() == 1);
            }
        }
        WHEN("there is an error inside a transaction") {
            {
                hades::transaction tr(conn, "one_transaction_test");
                bool thrown = false;
                try
                {
                    hades::devoid("INSERT INSERT INSERT INSERT INSERT", conn);
                }
                catch(const std::exception& e)
                {
                    // This exception will be thrown.
                    thrown = true;
                }
                REQUIRE(thrown);
                tr.commit();
            }
            THEN("the transaction had no effect") {
                styx::list l = hades::custom_select<site>(
                    conn,
                    "SELECT 1 FROM site "
                    );
                REQUIRE(l.size() == 0);
            }
            THEN("another transaction can run") {
                hades::transaction tr(conn, "one_transaction_test");
                hades::devoid("INSERT INTO site(name) VALUES('test')", conn);
                tr.commit();
                THEN("the transaction inserted a row") {
                    styx::list l = hades::custom_select<site>(
                        conn,
                        "SELECT 1 FROM site "
                        );
                    REQUIRE(l.size() == 1);
                }
            }
        }
    }
}

