#include "hades/get_column.hpp"

#include <cstring>

void hades::get_column(sqlite3_stmt *stmt, const int index, double& out)
{
    out = sqlite3_column_double(stmt, index);
}

void hades::get_column(sqlite3_stmt *stmt, const int index, int& out)
{
    out = sqlite3_column_int(stmt, index);
}

void hades::get_column(sqlite3_stmt *stmt, const int index, std::string& out)
{
    const char *sql_val = reinterpret_cast<const char*>(
            sqlite3_column_text(stmt, index)
            );
    if(sql_val == nullptr) out = "";
    else out = std::string(sql_val);
}

const char *hades::get_column_text(sqlite3_stmt *stmt, const int index)
{
    return reinterpret_cast<const char*>(
            sqlite3_column_text(stmt, index)
            );
}

void hades::get_column(
        sqlite3_stmt                *stmt,
        const int                   index,
        std::vector<unsigned char>& out
        )
{
    sqlite3_column_blob(stmt, index);
    int n_bytes = sqlite3_column_bytes(stmt, index);
    const void *p = sqlite3_column_blob(stmt, index);

    out.resize(n_bytes, 0);

    ::memcpy(reinterpret_cast<void*>(&(out[0])), p, n_bytes);
}

