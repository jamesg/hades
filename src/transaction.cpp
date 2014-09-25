#include "hades/transaction.hpp"

#include <iostream>
#include <sstream>
#include <stdexcept>

#include <sqlite3.h>

#include "hades/devoid.hpp"
#include "hades/mkstr.hpp"
#include "hades/row.hpp"

hades::transaction::transaction(
        hades::connection& connection,
        const std::string&  name
        ) :
    m_connection(connection),
    m_savepoint_name(name),
    m_released(false)
{
    // Start a SQLite TRANSACTION if outside of a transaction.
    if(m_connection.transaction_depth() == 0)
        devoid("BEGIN TRANSACTION", empty_row(), m_connection);
    else
        devoid(
                mkstr() << "SAVEPOINT " << m_savepoint_name,
                empty_row(),
                m_connection
                );
    m_connection.enter_transaction(this);
}

void hades::transaction::commit()
{
    if(m_released)
        throw std::runtime_error("Savepoint already released");

    m_connection.finish_transaction();

    if(m_connection.transaction_depth() == 0)
        devoid("COMMIT TRANSACTION", empty_row(), m_connection);
    else
        devoid(
            mkstr() << "RELEASE SAVEPOINT " << m_savepoint_name,
            empty_row(),
            m_connection
            );

    m_released = true;
}

void hades::transaction::rollback()
{
    if(m_released)
        throw std::runtime_error("Savepoint already released");

    if(m_connection.transaction_depth() == 0)
        devoid("ROLLBACK TRANSACTION", empty_row(), m_connection);
    else
        devoid(
            mkstr() << "ROLLBACK TO SAVEPOINT " <<
                m_connection.peek2_transaction()->m_savepoint_name,
            empty_row(),
            m_connection
            );

    m_connection.finish_transaction();
}

hades::transaction::~transaction()
{
    if(!m_released)
    {
        try
        {
            rollback();
        }
        catch(const std::exception& e)
        {
        }
    }
}

