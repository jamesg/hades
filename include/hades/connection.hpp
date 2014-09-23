#ifndef HADES_CONNECTION_HPP
#define HADES_CONNECTION_HPP

#include <list>
#include <string>

#include <boost/utility.hpp>

#include <sqlite3.h>

namespace hades
{
    class transaction;
    /*!
     * \brief Resource-managing class for connections to SQLite database files.
     */
    class connection : boost::noncopyable
    {
    public:
        /*!
         * \brief Open a connection to a new in-memory database.
         */
        static connection in_memory_database();
        /*!
         * \brief Attempt to open a connection to the database file.  Throws an
         * exception if the connection could not be established.
         */
        connection(std::string filename);
        connection(connection&& o);
        ~connection();
        /*!
         * \returns The internal SQLite handle representing the database
         * connection.  Required for running custom queries not assisted by
         * this class.
         */
        sqlite3 *handle();
        int transaction_depth();
        void enter_transaction(transaction*);
        void finish_transaction();
        /*!
         * \brief Get the transaction below the current one.
         *
         * Used for rolling back to a previous transaction state by name.
         */
        transaction *peek2_transaction();
    private:
        sqlite3 *m_handle;
        std::list<transaction*> m_transactions;
    };
}

#endif

