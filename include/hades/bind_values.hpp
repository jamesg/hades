#ifndef HADES_BIND_VALUES_HPP
#define HADES_BIND_VALUES_HPP

#ifdef HADES_ENABLE_DEBUGGING
#include <iostream>
#endif

#include <algorithm>
#include <stdexcept>

#include <boost/fusion/include/fold.hpp>
#include <boost/optional.hpp>
#include <sqlite3.h>

#include "styx/element.hpp"

namespace hades
{
    /*!
     * \brief Bind a value of any styx::element type to a sqlite statement.
     *
     * \note Not all types are supported.
     *
     * \throws std::runtime_error if the input type is not supported or there
     * was a SQLite error binding the value.
     */
    void bind_any(int index, const styx::element&, sqlite3_stmt *stmt);

    void bind(int index, const std::string& s, sqlite3_stmt *stmt);
    void bind(int index, const double value, sqlite3_stmt *stmt);
    void bind(int index, const styx::int_type value, sqlite3_stmt *stmt);
    void bind_null(int index, sqlite3_stmt *stmt);
    /*!
     * \brief Bind a BLOB to a SQLite statement.
     * \param l Length of the data pointed to by 'data'.
     */
    void bind(int index, const void *data, int l, sqlite3_stmt *stmt);

    namespace detail
    {
        /*!
         * \internal
         * \brief Functor to bind values from a Fusion container to
         * placeholders in a SQL statement.
         */
        class bind_fusion
        {
            public:
                bind_fusion(sqlite3_stmt *stmt) :
                    m_stmt(stmt)
                {
                }

                typedef int result_type;

                template<typename T>
                int operator()(const int index, const T& val) const
                {
#ifdef HADES_ENABLE_DEBUGGING
                    std::cerr << "hades::detail::bind_fusion bind " << val <<
                        " to " << index << std::endl;
#endif
                    bind(index, val, m_stmt);
                    return index + 1;
                }

            private:
                sqlite3_stmt *m_stmt;
        };
    }

    /*!
     * \brief Bind the values in a fusion container 'container' into a
     * statement.  Values in the container are substituted for ?'s in the SQL
     * statement.
     */
    template<typename Values>
    void bind_values(const Values& container, sqlite3_stmt *stmt)
    {
        // 1 because the leftmost parameter in a sqlite prepared statement has
        // index 1
        boost::fusion::fold(container, 1, detail::bind_fusion(stmt));
    }
}

#endif

