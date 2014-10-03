#ifndef HADES_GET_COLLECTION_HPP
#define HADES_GET_COLLECTION_HPP

#include <sqlite3.h>

#include "hades/connection.hpp"
#include "hades/filter.hpp"
#include "hades/mkstr.hpp"
#include "hades/retrieve_values.hpp"
#include "hades/tuple.hpp"

namespace hades
{
    class connection;

    /*!
     * \brief Get every tuple of a relation matching a filter condition.
     *
     * \param Tuple A type deriving from hades::relation and hades::tuple.
     * Determines the relation to query and the fields queried.
     */
    template<typename Tuple>
    std::vector<Tuple> get_collection_vector(connection& conn, const basic_filter& filter)
    {
        std::vector<Tuple> vector;

        std::ostringstream query;
        query << "SELECT ";
        Tuple::attribute_list_type::column_list(query);
        query << " FROM " << Tuple::relation_name;
        query << filter.clause();

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

        while(sqlite3_step(stmt) == SQLITE_ROW)
        {
            try
            {
                Tuple out;
                Tuple::attribute_list_type::retrieve_values(stmt, out);
                vector.push_back(out);
            }
            catch(const std::exception&)
            {
                sqlite3_finalize(stmt);
                throw;
            }
        }

        if(sqlite3_finalize(stmt) != SQLITE_OK)
            throw std::runtime_error("finalizing SQLite statement");

        return vector;
    }
    /*!
     * \brief Get every tuple of a relation.
     *
     * \param Tuple A type deriving from hades::relation and hades::tuple.
     * Determines the relation to query and the fields queried.
     */
    template<typename Tuple>
    std::vector<Tuple> get_collection_vector(connection& conn)
    {
        return get_collection_vector<Tuple>(conn, filter_all());
    }
    /*!
     * \brief Get every tuple of a relation matching a filter condition.
     *
     * \param Tuple A type deriving from hades::relation and hades::tuple.
     * Determines the relation to query and the fields queried.
     */
    template<typename Tuple>
    styx::list get_collection(connection& conn, const basic_filter& filter)
    {
        styx::list list;

        std::ostringstream query;
        query << "SELECT ";
        Tuple::attribute_list_type::column_list(query);
        query << " FROM " << Tuple::relation_name;
        query << filter.clause();

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

        while(sqlite3_step(stmt) == SQLITE_ROW)
        {
            try
            {
                Tuple out;
                Tuple::attribute_list_type::retrieve_values(stmt, out);
                list.append(out.get_element());
            }
            catch(const std::exception&)
            {
                sqlite3_finalize(stmt);
                throw;
            }
        }

        if(sqlite3_finalize(stmt) != SQLITE_OK)
            throw std::runtime_error("finalizing SQLite statement");

        return list;
    }
    /*!
     * \brief Get every tuple of a relation.
     *
     * \param Tuple A type deriving from hades::relation and hades::tuple.
     * Determines the relation to query and the fields queried.
     */
    template<typename Tuple>
    styx::list get_collection(connection& conn)
    {
        return get_collection<Tuple>(conn, filter_all());
    }
}

#endif

