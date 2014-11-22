#include "hades/step.hpp"

#include <sstream>
#include <stdexcept>

#include "hades/connection.hpp"
#include "hades/exception.hpp"

void hades::step(sqlite3_stmt *stmt)
{
    int ret;
    while(true)
    {
        ret = sqlite3_step(stmt);
        switch(ret)
        {
            case SQLITE_DONE:
            case SQLITE_ROW:
            case SQLITE_OK:
                return;
            case SQLITE_BUSY:
                sqlite3_sleep(250);
                break;
            case SQLITE_ERROR:
                throw hades::exception("Stepping SQLite query");
            default:
                throw hades::exception("Unhandled SQLITE return code");
        }
    }
}

void hades::step(sqlite3_stmt *stmt, connection& conn)
{
    int ret;
    while(true)
    {
        ret = sqlite3_step(stmt);
        switch(ret)
        {
            case SQLITE_DONE:
            case SQLITE_ROW:
            case SQLITE_OK:
                return;
            case SQLITE_BUSY:
                sqlite3_sleep(250);
                break;
            case SQLITE_ERROR:
                {
                    std::ostringstream oss;
                    oss << "stepping sqlite query: ";
                    oss << sqlite3_errmsg(conn.handle());
                    throw hades::exception(oss.str());
                }
            default:
                throw hades::exception("Unhandled SQLITE return code");
        }
    }
}

