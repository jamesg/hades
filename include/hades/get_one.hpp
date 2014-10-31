#ifndef HADES_GET_ONE_HPP
#define HADES_GET_ONE_HPP

#include <sqlite3.h>

#include "hades/connection.hpp"
#include "hades/filter.hpp"
#include "hades/mkstr.hpp"
#include "hades/retrieve_values.hpp"
#include "hades/tuple.hpp"

namespace hades
{
    /*!
     * \brief Get a single tuple matching a hades::filter.  Throws an exception
     * if no such tuple is found.
     *
     * \throws std::exception
     */
    template<typename Tuple>
    Tuple get_one(connection& conn, basic_filter& filter)
    {
        std::ostringstream query;
        query << "SELECT ";
        Tuple::attribute_list_type::column_list(query);
        query << " FROM " << Tuple::relation_name;
        query << " " << filter.clause();

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
            throw std::runtime_error(
                    mkstr() << "preparing SQLite statement \"" <<
                        query.str() << "\""
                    );
        }
        filter.bind(stmt);

        if(sqlite3_step(stmt) == SQLITE_ROW)
        {
            try
            {
                Tuple out;
                Tuple::attribute_list_type::retrieve_values(stmt, out);
                sqlite3_finalize(stmt);
                return out;
            }
            catch(const std::exception&)
            {
                sqlite3_finalize(stmt);
                throw;
            }
            if(sqlite3_step(stmt) == SQLITE_ROW)
                throw std::runtime_error(
                    mkstr() <<
                    "hades::get_one: SELECT yielded multiple rows: \"" <<
                    query.str() << "\""
                    );
        }
        else
        {
            sqlite3_finalize(stmt);
            throw std::runtime_error(
                mkstr() <<
                "hades::get_one: SELECT yielded zero rows: \"" <<
                query.str() << "\""
                );
        }
    }
    /*!
     * \brief Get a single tuple matching a hades::filter.  Throws an exception
     * if no such tuple is found.
     *
     * \note As this overload does not accept a hades::basic_filter argument,
     * it is only useful for types of which there is only one instance, such as
     * configuration types.
     *
     * \throws std::exception
     */
    template<typename Tuple>
    Tuple get_one(connection& conn)
    {
        filter_all all;
        return get_one<Tuple>(conn, all);
    }
}

#endif
