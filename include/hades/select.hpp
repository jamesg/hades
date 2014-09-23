#ifndef HADES_SELECT_HPP
#define HADES_SELECT_HPP

#include "hades/bind_values.hpp"
#include "hades/mkstr.hpp"
#include "hades/retrieve_values.hpp"
#include "hades/tuple.hpp"

namespace hades
{
    /*!
     * \brief Execute a SQL SELECT query retrieving all attributes of the tuple
     * type 'Tuple'.
     *
     * \param Tuples A type inheriting from hades::tuple and hades::relation.
     * Determines the attributes and relation that will be queried.
     */
    template<typename Tuple, typename Where>
    styx::list select(
            connection& conn,
            const Where& where_
            )
    {
        styx::list list;

        std::ostringstream query;
        query << "SELECT ";
        Tuple::attribute_list_type::column_list(query);
        query << " FROM " << Tuple::relation_name;
        query << " WHERE " << where_.clause();

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

        where_.bind(stmt);

        while(sqlite3_step(stmt) == SQLITE_ROW)
        {
            try
            {
                styx::element element;
                Tuple out(element);
                Tuple::attribute_list_type::retrieve_values(stmt, out);
                list.append(element);
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

