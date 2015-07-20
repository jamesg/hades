#ifndef HADES_DEVOID_HPP
#define HADES_DEVOID_HPP

#include <algorithm>
#include <vector>
#include <sstream>
#include <string>

#include <boost/optional.hpp>

#include "hades/bind_values.hpp"
#include "hades/connection.hpp"
#include "hades/exception.hpp"
#include "hades/mkstr.hpp"

namespace hades
{
    /*!
     * \brief Execute a query that is not expected to return a result (or
     * returns a result that we are not interested in).
     *
     * \returns The number of rows affected by the query, if the query is an
     * INSERT, DELETE or UPDATE.  For any other type of query, the result is
     * undefined.  The result is also undefined if multiple threads are
     * accessing the database.
     *
     * \throws hades::exception when an SQL error is encountered.
     */
    int devoid(const std::string& query, connection& db);
    /*!
     * \brief Execute a query that is not expected to return a result (or
     * returns a result that we are not interested in).
     *
     * \param values A Boost Fusion container of values to be bound to question
     * mark placeholders in the SQL statement.  A useful container is
     * hades::row.
     *
     * \returns The number of rows affected by the query, if the query is an
     * INSERT, DELETE or UPDATE.  For any other type of query, the result is
     * undefined.  The result is also undefined if multiple threads are
     * accessing the database.
     *
     * \throws hades::exception when an SQL error is encountered.
     */
    template <typename T>
    int devoid(const std::string& query, const T& values, connection& db)
    {
        sqlite3_stmt *stmt = nullptr;
        sqlite3_prepare(db.handle(), query.c_str(), -1, &stmt, nullptr);
        if(stmt == nullptr)
            throw hades::exception(
                mkstr() << "preparing SQL statement \"" << query << "\": " <<
                    sqlite3_errmsg(db.handle())
            );
        bind_values(values, stmt);
        int step_ret = sqlite3_step(stmt);
        int finalise_ret = sqlite3_finalize(stmt);
        if(step_ret != SQLITE_DONE)
            throw hades::exception(
                mkstr() << "stepping devoid SQL query \"" << query <<
                    "\": " << sqlite3_errmsg(db.handle())
                );
        if(finalise_ret != SQLITE_OK)
            throw hades::exception(
                mkstr() << "finalising devoid SQL query \"" << query <<
                    "\": " << sqlite3_errmsg(db.handle())
                );
        return sqlite3_changes(db.handle());
    }
}

#endif
