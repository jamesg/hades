#include "catch.hpp"

#include "hades/connection.hpp"
#include "hades/crud.ipp"
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
        public hades::has_candidate_key<attribute::site_id>,
        public hades::crud<site>
    {
        site()
        {
        }

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

    hades::connection get_database()
    {
        hades::connection conn = hades::connection::in_memory_database();
        hades::devoid(
                "CREATE TABLE site ( "
                " site_id INTEGER PRIMARY KEY AUTOINCREMENT, "
                " name VARCHAR "
                " ) ",
                hades::empty_row(),
                conn
                );
        return conn;
    }
}

SCENARIO("hades::transaction") {
    GIVEN("a simple database") {
        hades::connection conn = get_database();
        WHEN("one transaction is run but not committed") {
            {
                hades::transaction tr(conn, "one_transaction_test");
                hades::devoid("INSERT INTO site(name) VALUES('test')", conn);
            }
            THEN("the transaction had no effect") {
                styx::list l = site::get_collection(conn);
                REQUIRE(l.size() == 0);
            }
        }
    }
    GIVEN("a simple database") {
        hades::connection conn = get_database();
        WHEN("one transaction is run and committed") {
            {
                hades::transaction tr(conn, "one_transaction_test");
                hades::devoid("INSERT INTO site(name) VALUES('test')", conn);
                tr.commit();
            }
            THEN("the transaction inserted a row") {
                styx::list l = site::get_collection(conn);
                REQUIRE(l.size() == 1);
            }
        }
    }
    GIVEN("a simple database") {
        hades::connection conn = get_database();
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
                styx::list l = site::get_collection(conn);
                REQUIRE(l.size() == 0);
            }
            THEN("another transaction can run") {
                hades::transaction tr(conn, "one_transaction_test");
                hades::devoid("INSERT INTO site(name) VALUES('test')", conn);
                tr.commit();
                THEN("the transaction inserted a row") {
                    styx::list l = site::get_collection(conn);
                    REQUIRE(l.size() == 1);
                }
            }
        }
    }
    GIVEN("a simple database") {
        hades::connection conn = get_database();
        WHEN("transactions are nested and the inner transaction fails") {
            hades::transaction tr_outer(conn, "outer_transaction");
            {
                hades::transaction tr_inner(conn, "outer_transaction");
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
                tr_inner.commit();
            }
            hades::devoid("INSERT INTO site(name) VALUES('test')", conn);
            tr_outer.commit();
            THEN("the outer transaction inserted a row") {
                styx::list l = site::get_collection(conn);
                REQUIRE(l.size() == 1);
            }
        }
    }
    GIVEN("a simple database") {
        hades::connection conn = get_database();
        WHEN("transactions are nested and the outer transaction fails") {
            hades::transaction tr_outer(conn, "outer_transaction");
            {
                hades::transaction tr_inner(conn, "outer_transaction");
                hades::devoid("INSERT INTO site(name) VALUES('test')", conn);
                tr_inner.commit();
            }
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
            tr_outer.commit();
            THEN("no rows were inserted") {
                styx::list l = site::get_collection(conn);
                REQUIRE(l.size() == 1);
            }
        }
    }
}

