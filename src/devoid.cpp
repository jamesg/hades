#include "hades/devoid.hpp"

#include "hades/exception.hpp"

void hades::devoid(const std::string& query, connection& db)
{
    sqlite3_stmt *stmt = nullptr;
    sqlite3_prepare(db.handle(), query.c_str(), -1, &stmt, nullptr);
    if(stmt == nullptr)
        throw hades::exception(
            mkstr() << "preparing devoid SQL query \"" << query << "\": " <<
                sqlite3_errmsg(db.handle())
            );
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
}

