#ifndef HADES_GET_BY_ID_HPP
#define HADES_GET_BY_ID_HPP

#ifdef HADES_ENABLE_DEBUGGING
#include <iostream>
#endif
#include <sqlite3.h>

#include "styx/serialisers/vector.hpp"

#include "hades/bind_values.hpp"
#include "hades/compound_id.hpp"
#include "hades/connection.hpp"
#include "hades/crud/get_flags.ipp"
#include "hades/mkstr.hpp"
#include "hades/retrieve_values.hpp"
#include "hades/transaction.hpp"

namespace hades
{
    /*!
     * \brief Get a tuple from the database with every attribute in the
     * candidate key matching the attribute in 'id'.
     *
     * \param Out Database struct inheriting from has_candidate_key, tuple
     * and relation.
     * \param id Compound id (instance of a candidate key) to select from
     * the database relation.
     */
    template<typename Out>
    void get_by_id(
            connection& conn,
            typename Out::id_type id,
            Out& out
            )
    {
        hades::transaction transaction(conn, "hades_get_by_id");
        std::vector<Out> out_vector;
        std::ostringstream oss;
        oss << "SELECT ";
        styx::serialise(
                Out::attribute_list_type::to_vector(),
                [](const char *s, std::ostream& os) {
                    os << s;
                },
                ", ",
                oss
                );
        oss << " FROM " << Out::relation_name;
        oss << " WHERE ";
        Out::id_type::key_where_clause(oss);
#ifdef HADES_ENABLE_DEBUGGING
        std::cerr << "hades get_by_id query: " << oss. str() << std::endl;
#endif

        sqlite3_stmt *stmt = nullptr;
        if(
                sqlite3_prepare(
                    conn.handle(),
                    oss.str().c_str(),
                    oss.str().length(),
                    &stmt,
                    nullptr
                    ) != SQLITE_OK
                )
        {
            std::ostringstream oss_;
            oss_ << "error in SQLite select; query: \"" << oss.str() <<
                "\" sqlite error: " << sqlite3_errmsg(conn.handle()) << std::endl;
            throw std::runtime_error(oss_.str());
        }

        // First parameter is index 1.
        Out::id_type::bind_key_values(id, stmt);

        if(sqlite3_step(stmt) == SQLITE_ROW)
        {
            Out::attribute_list_type::retrieve_values(stmt, out);
        }
        else
        {
            sqlite3_finalize(stmt);
            throw std::runtime_error(
                    mkstr() << "SELECT did not yield one row: \"" <<
                        oss.str() << "\""
                        );
        }

        if( sqlite3_finalize(stmt) != SQLITE_OK )
            throw std::runtime_error("finalizing sqlite statement");

        detail::get_flags<Out>(conn, out);
    }

    template<typename Out>
    Out get_by_id(
            connection& conn,
            typename Out::id_type id
            )
    {
        Out out;
        get_by_id(conn, id, out);
        return out;
    }
}

#endif

