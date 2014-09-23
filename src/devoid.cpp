#include "hades/devoid.hpp"

void hades::devoid(const std::string& query, connection& db)
{
    sqlite3_stmt *stmt = nullptr;
    sqlite3_prepare(db.handle(), query.c_str(), -1, &stmt, nullptr);
    int step_ret     = sqlite3_step(stmt);
    int finalise_ret = sqlite3_finalize(stmt);
    if( step_ret != SQLITE_DONE )
        throw std::runtime_error(
            mkstr() << "stepping devoid SQL query \"" << query <<
                "\": " << sqlite3_errmsg(db.handle())
            );
    if( finalise_ret != SQLITE_OK )
        throw std::runtime_error(
            mkstr() << "finalising devoid SQL query \"" << query <<
                "\": " << sqlite3_errmsg(db.handle())
            );
}

