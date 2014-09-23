#ifndef HADES_GET_COLLECTION_HPP
#define HADES_GET_COLLECTION_HPP

#include <sqlite3.h>

#include "hades/connection.hpp"
#include "hades/mkstr.hpp"
#include "hades/retrieve_values.hpp"
#include "hades/tuple.hpp"

namespace hades
{
    class connection;

    /*!
     * \brief Get every tuple of a relation.
     *
     * \param Tuple A type deriving from hades::relation and hades::tuple.
     * Determines the relation to query and the fields queried.
     */
    template<typename Tuple>
    styx::list get_collection(connection& conn, const std::string& where = "1")
    {
        styx::list list;

        std::ostringstream query;
        query << "SELECT ";
        Tuple::attribute_list_type::column_list(query);
        query << " FROM " << Tuple::relation_name;
        query << " WHERE " << where;

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
}

#endif

