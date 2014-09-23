#ifndef HADES_GET_COLUMN_HPP
#define HADES_GET_COLUMN_HPP

#include "hades/bind_values.hpp"

#include <vector>

namespace hades
{
    void get_column(sqlite3_stmt *stmt, const int index, double& out);
    void get_column(sqlite3_stmt *stmt, const int index, int& out);
    void get_column(sqlite3_stmt *stmt, const int index, std::string& out);
    const char *get_column_text(sqlite3_stmt *stmt, const int index);
    void get_column(
            sqlite3_stmt                *stmt,
            const int                   index,
            std::vector<unsigned char>& out
            );
}

#endif

