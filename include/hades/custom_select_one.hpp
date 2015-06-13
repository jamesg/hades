#ifndef HADES_CUSTOM_SELECT_ONE_HPP
#define HADES_CUSTOM_SELECT_ONE_HPP

#include "styx/list.hpp"

#include "hades/custom_select.hpp"

namespace hades
{
    /*!
     * \brief Execute a custom SELECT statement that is expected to return
     * exactly one tuple.  An exception is thrown if zero or more than one
     * tuples are returned.
     */
    template<typename Tuple, typename Values, const char *...Attributes>
    Tuple custom_select_one(
            hades::connection& conn,
            const std::string& query,
            const Values& values
            )
    {
        sqlite3_stmt *stmt = nullptr;
        if( sqlite3_prepare(
                    conn.handle(),
                    query.c_str(),
                    -1,
                    &stmt,
                    nullptr
                    ) != SQLITE_OK )
        {
            throw hades::exception(
                mkstr() << "preparing SQLite statement for custom_select \"" <<
                    query << "\": " << sqlite3_errmsg(conn.handle())
                );
        }

        bind_values(values, stmt);

        Tuple out;
        if(sqlite3_step(stmt) == SQLITE_ROW)
            attribute_list<Attributes...>::retrieve_values(stmt, out);
        else
            throw hades::exception(
                mkstr() <<
                "hades::custom_select_one zero rows in result of \"" << query <<
                "\""
                );
        if(sqlite3_step(stmt) == SQLITE_ROW)
            throw hades::exception(
                mkstr() <<
                "hades::custom_select_one more than one row in result of \"" <<
                query << "\""
                );
        return out;
    }
}

#endif

