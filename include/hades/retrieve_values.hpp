#ifndef HADES_RETRIEVE_VALUES_HPP
#define HADES_RETRIEVE_VALUES_HPP

#include <stdexcept>

#include <boost/fusion/include/fold.hpp>

#include <sqlite3.h>

#include "hades/get_column.hpp"

namespace hades
{
    struct retrieve_fusion
    {
        retrieve_fusion(sqlite3_stmt *stmt) :
            m_stmt(stmt)
        {
        }

        typedef int result_type;

        template<typename T>
        int operator()(const int index, T& val) const
        {
            get_column(m_stmt, index, val);
            return index + 1;
        }

        private:
        sqlite3_stmt *m_stmt;
    };

    /*!
     * \brief Copy SQLite column values into Fusion container 'out'.
     */
    template <typename T>
    void retrieve_values(sqlite3_stmt *stmt, T& out)
    {
        // 0 because the first column has index 0
        boost::fusion::fold(out, 0, hades::retrieve_fusion(stmt));
    }
}

#endif

