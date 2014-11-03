#ifndef HADES_EXISTS_HPP
#define HADES_EXISTS_HPP

#ifdef HADES_ENABLE_DEBUGGING
#include <iostream>
#endif
#include "hades/connection.hpp"
#include "hades/filter.hpp"
#include "hades/mkstr.hpp"
#include "hades/retrieve_values.hpp"
#include "hades/tuple.hpp"

namespace hades
{
    /*!
     * \brief Determine whether or not the database contains a tuple matching a
     * given query.
     *
     * \throws std::exception
     */
    template<typename Tuple>
    bool exists(connection& conn, basic_filter& filter)
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

        auto step = sqlite3_step(stmt);
#ifdef HADES_ENABLE_DEBUGGING
        std::cerr << "hades::exists: query \"" << query.str() <<
            "\", exists: " << std::boolalpha << (step == SQLITE_ROW) <<
            std::endl;
#endif
        sqlite3_finalize(stmt);
        return (step == SQLITE_ROW);
    }
    /*!
     * \brief Determine whether or not the database contains a tuple matching a
     * given query.
     *
     * \throws std::exception
     */
    template<typename Tuple>
    bool exists(connection& conn)
    {
        filter_all all;
        return exists<Tuple>(conn, all);
    }
}

#endif

