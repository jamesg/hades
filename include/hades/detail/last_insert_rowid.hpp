#ifndef HADES_DETAIL_LAST_INSERT_ROWID_HPP
#define HADES_DETAIL_LAST_INSERT_ROWID_HPP

namespace hades
{
    class connection;

    namespace detail
    {
        /*!
         * \brief Get the id of the last inserted row using SQLite's native
         * last_insert_rowid() function.
         */
        int last_insert_rowid(hades::connection&);
    }
}

#endif

