#ifndef HADES_TRANSACTION_HPP
#define HADES_TRANSACTION_HPP

#include <string>

#include "hades/connection.hpp"

namespace hades
{
    class transaction
    {
    public:
        /*!
         * \brief Open a new transaction with a named savepoint (so it can be
         * rolled back or committed in the destructor).
         */
        transaction(hades::connection&, const std::string& name);
        ~transaction();
        void commit();
        void rollback();
    private:
        hades::connection& m_connection;
        std::string m_savepoint_name;
        bool m_released;
    };
}

#endif

