#include "hades/detail/last_insert_rowid.hpp"

#include "hades/connection.hpp"
#include "hades/get_column.hpp"
#include "hades/step.hpp"

int hades::detail::last_insert_rowid(hades::connection& db)
{
    sqlite3_stmt *stmt = nullptr;
    sqlite3_prepare(
            db.handle(),
            "SELECT last_insert_rowid()",
            -1,
            &stmt,
            nullptr
            );
    hades::step(stmt);
    int rowid = 0;
    hades::get_column(stmt, 0, rowid);
    sqlite3_finalize(stmt);
    return rowid;
}
