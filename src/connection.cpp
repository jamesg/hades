#include "hades/connection.hpp"

#include <stdexcept>

hades::connection hades::connection::in_memory_database()
{
    connection c(":memory:");
    return c;
}

hades::connection::connection(std::string filename) :
    m_handle(nullptr)
{
    // Attempt to open a connection
    if(sqlite3_open(filename.c_str(), &m_handle) != SQLITE_OK)
    {
        if(m_handle)
        {
            sqlite3_close(m_handle);
            m_handle = nullptr;
        }
        throw std::runtime_error("Unable to open SQLite connection");
    }

    // Enable foreign keys
    sqlite3_stmt *stmt = nullptr;
    sqlite3_prepare(m_handle, "PRAGMA foreign_keys = ON", -1, &stmt, nullptr);
    int ret = sqlite3_step(stmt);
    if( ret != SQLITE_DONE && ret != SQLITE_OK )
    {
        throw std::runtime_error("enabling SQLite foreign key processing");
    }
    sqlite3_finalize(stmt);
}

hades::connection::connection(connection&& o) :
    m_handle(o.m_handle)
{
    o.m_handle = nullptr;
}

hades::connection::~connection()
{
    if(m_handle)
        sqlite3_close(m_handle);
}

sqlite3 *hades::connection::handle()
{
    return m_handle;
}

int hades::connection::transaction_depth()
{
    return m_transactions.size();
}

void hades::connection::enter_transaction(transaction *t)
{
    m_transactions.push_front(t);
}

void hades::connection::finish_transaction()
{
    m_transactions.pop_front();
}

hades::transaction *hades::connection::peek2_transaction()
{
    auto it = m_transactions.begin();
    if(it != m_transactions.end())
        ++it;
    if(it != m_transactions.end())
        return *it;
    return nullptr;
}

