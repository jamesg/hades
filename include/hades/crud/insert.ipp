#ifndef HADES_CRUD_INSERT_IPP
#define HADES_CRUD_INSERT_IPP

#ifdef HADES_ENABLE_DEBUGGING
#include <iostream>
#endif
#include <sstream>
#include <sqlite3.h>

#include "hades/crud.hpp"
#include "hades/crud/save_flags.ipp"
#include "hades/connection.hpp"
#include "hades/bind_values.hpp"
#include "hades/exception.hpp"
#include "hades/detail/last_insert_rowid.hpp"
#include "hades/mkstr.hpp"
#include "styx/serialisers/vector.hpp"
#ifdef HADES_ENABLE_DEBUGGING
#include "styx/serialise_json.hpp"
#endif

namespace hades
{
    class connection;
}

/*!
 * \brief Insert the tuple into the database.  The id of this tuple
 * will be set to the id of the database tuple.
 */
template<typename Tuple>
void hades::crud<Tuple>::insert(connection& conn)
{
    Tuple& t = static_cast<Tuple&>(*this);
    {
        std::ostringstream query;
        query << "INSERT INTO " << Tuple::relation_name << " ( ";
        auto attributes = t.value_attributes();
        styx::serialise(
                attributes,
                [](const char *s, std::ostream& os) {
                    os << s;
                },
                ", ",
                query
                );
        query << " ) VALUES ( ";
        styx::serialise(
                attributes,
                [](const char *, std::ostream& os) {
                    os << "?";
                },
                ", ",
                query
                );
        query << ")";

#ifdef HADES_ENABLE_DEBUGGING
        std::cerr << "hades insert query: " << query.str() << std::endl;
#endif

        sqlite3_stmt *stmt = nullptr;
        if(
                sqlite3_prepare(
                    conn.handle(),
                    query.str().c_str(),
                    -1,
                    &stmt,
                    nullptr
                    ) != SQLITE_OK
                )
        {
            throw hades::exception(
                    mkstr() << "preparing SQLite statement \"" <<
                        query.str() << "\" for insert (" <<
                        sqlite3_errmsg(conn.handle()) << ")"
                    );
        }

        try
        {
            t.bind_set_values(stmt);
        }
        catch(const std::exception& e)
        {
            throw hades::exception(
                    mkstr() << "binding values to insert: " << e.what() <<
                    " query   \"" << query.str() << "\""
                    );
        }

        int step_ret = sqlite3_step(stmt);
        if(step_ret != SQLITE_OK && step_ret != SQLITE_DONE)
        {
            std::ostringstream error_string;
            error_string << "stepping SQLite insert " << query.str() <<
                " " << sqlite3_errmsg(conn.handle());
            throw hades::exception(error_string.str());
        }

        int finalise_ret = sqlite3_finalize(stmt);
        if(finalise_ret != SQLITE_OK && finalise_ret != SQLITE_DONE)
        {
            std::ostringstream error_string;
            error_string << "SQLite finalise " << query.str() << " " <<
                sqlite3_errmsg(conn.handle());
            throw hades::exception(error_string.str());
        }
    }

    // Only attempt to retrieve the complete id if the type has id
    // attributes.
    if(Tuple::id_type::arity > 0)
    {
        int last_id = detail::last_insert_rowid(conn);
        std::ostringstream id_query;
        id_query << "SELECT ";
        Tuple::id_type::key_column_list(id_query);
        id_query << " FROM " << Tuple::relation_name;
        id_query << " WHERE rowid = ? ";
        sqlite3_stmt *last_id_stmt = nullptr;
        if(
                sqlite3_prepare(
                    conn.handle(),
                    id_query.str().c_str(),
                    -1,
                    &last_id_stmt,
                    nullptr
                    ) != SQLITE_OK
                )
        {
            throw hades::exception(
                    mkstr() << "preparing id query \"" << id_query.str() << "\""
                    );
        }

        try
        {
            hades::bind(1, (styx::int_type)last_id, last_id_stmt);
        }
        catch(const std::exception& e)
        {
            throw hades::exception(
                    mkstr() << "binding to id statement: " << e.what()
                    );
        }

        int step_ret = sqlite3_step(last_id_stmt);
        if(step_ret != SQLITE_ROW)
        {
            throw hades::exception(
                mkstr() << "stepping SQLite select last id \"" <<
                    id_query.str() << "\" " << sqlite3_errmsg(conn.handle())
                );
        }

        typename Tuple::id_type id =
            Tuple::id_type::from_stmt(last_id_stmt);
        t.set_id(id);
#ifdef HADES_ENABLE_DEBUGGING
        std::cerr << "hades::crud::insert id " << styx::serialise_json(id) << std::endl;
        std::cerr << "hades::crud::insert new tuple " << styx::serialise_json(t) << std::endl;
#endif

        int finalise_ret = sqlite3_finalize(last_id_stmt);
        if(finalise_ret != SQLITE_OK && finalise_ret != SQLITE_DONE)
        {
            std::ostringstream error_string;
            error_string << "SQLite finalise " << id_query.str() << " " <<
                sqlite3_errmsg(conn.handle());
            throw hades::exception(error_string.str());
        }

        detail::save_flags<Tuple>(static_cast<Tuple&>(*this), conn);
    }
}

#endif

