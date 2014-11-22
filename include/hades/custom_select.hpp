#ifndef HADES_CUSTOM_SELECT_HPP
#define HADES_CUSTOM_SELECT_HPP

#include <iterator>
#include <set>

#include <sqlite3.h>

#include "styx/list.hpp"
#include "styx/object_accessor.hpp"
#include "hades/attribute_list.hpp"
#include "hades/bind_values.hpp"
#include "hades/connection.hpp"
#include "hades/exception.hpp"
#include "hades/mkstr.hpp"

namespace hades
{
    namespace detail
    {
        template<typename OutputIterator, typename Tuple, const char *...Attributes>
        void custom_select(
                hades::connection& conn,
                const std::string& query,
                OutputIterator it
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

            while(sqlite3_step(stmt) == SQLITE_ROW)
            {
                Tuple tuple;
                attribute_list<Attributes...>::retrieve_values(stmt, tuple);
                *it = tuple;
            }

            if(sqlite3_finalize(stmt) != SQLITE_OK)
                throw hades::exception("Finalizing SQLite statement");
        }
        template<typename OutputIterator, typename Tuple, typename Values, const char *...Attributes>
        void custom_select(
                hades::connection& conn,
                const std::string& query,
                const Values& values,
                OutputIterator it
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

            while(sqlite3_step(stmt) == SQLITE_ROW)
            {
                Tuple tuple;
                attribute_list<Attributes...>::retrieve_values(stmt, tuple);
                *it = tuple;
            }

            if(sqlite3_finalize(stmt) != SQLITE_OK)
                throw hades::exception("Finalizing SQLite statement");
        }
    }
    template<typename Tuple, const char *...Attributes>
    std::set<Tuple> custom_select_set(
            hades::connection& conn,
            const std::string& query
            )
    {
        std::set<Tuple> out;
        detail::custom_select<std::insert_iterator<std::set<Tuple>>, Tuple, Attributes...>(
                conn,
                query,
                std::inserter(out, out.end())
                );
        return out;
    }
    template<typename Tuple, typename Values, const char *...Attributes>
    std::set<Tuple> custom_select_set(
            hades::connection& conn,
            const std::string& query,
            Values values
            )
    {
        std::set<Tuple> out;
        detail::custom_select<std::insert_iterator<std::set<Tuple>>, Tuple, Values, Attributes...>(
                conn,
                query,
                values,
                std::inserter(out, out.end())
                );
        return out;
    }
    /*!
     * \brief Execute a custom SELECT statement.
     *
     * \param Tuple Tuple type used to unpack the result.  Must derive from
     * hades::tuple.
     * \param Attributes List of attributes in the output tuple to fill with
     * data from the SQLite row.
     */
    template<typename Tuple, const char *...Attributes>
    styx::list custom_select(
            hades::connection& conn,
            const std::string& query
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

        styx::list list;
        while(sqlite3_step(stmt) == SQLITE_ROW)
        {
            styx::object_accessor accessor;
            attribute_list<Attributes...>::retrieve_values(stmt, accessor);
            list.append(accessor.get_element());
        }

        if(sqlite3_finalize(stmt) != SQLITE_OK)
            throw hades::exception("Finalizing SQLite statement");

        return list;
    }
    /*!
     * \brief Execute a custom SELECT statement.
     *
     * \param Tuple Tuple type used to unpack the result.  Must derive from
     * hades::tuple.
     * \param Values A Boost Fusion container type.  hades::row is an easy to
     * use container.
     * \param Attributes List of attributes in the output tuple to fill with
     * data from the SQLite row.
     * \param values Boost Fusion container of values to bind to placeholders
     * in the query.
     */
    template<typename Tuple, typename Values, const char *...Attributes>
    styx::list custom_select(
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

        styx::list list;
        while(sqlite3_step(stmt) == SQLITE_ROW)
        {
            styx::object_accessor accessor;
            attribute_list<Attributes...>::retrieve_values(stmt, accessor);
            list.append(accessor.get_element());
        }

        if(sqlite3_finalize(stmt) != SQLITE_OK)
            throw hades::exception("Finalizing SQLite statement");

        return list;
    }
}

#endif

