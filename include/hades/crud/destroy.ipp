#ifndef HADES_CRUD_DESTROY_IPP
#define HADES_CRUD_DESTROY_IPP

#ifdef HADES_ENABLE_DEBUGGING
#include <iostream>
#endif
#include <sstream>
#include <stdexcept>
#include <sqlite3.h>

#include "hades/connection.hpp"
#include "hades/mkstr.hpp"

template<typename Tuple>
bool hades::crud<Tuple>::destroy(connection& conn)
{
    std::ostringstream oss;
    oss << "DELETE FROM " << Tuple::relation_name << " WHERE ";
    Tuple::id_type::key_where_clause(oss);
#ifdef HADES_ENABLE_DEBUGGING
    std::cerr << "hades delete query: " << oss.str() << std::endl;
#endif

    sqlite3_stmt *stmt = nullptr;
    if(
            sqlite3_prepare(
                conn.handle(),
                oss.str().c_str(),
                oss.str().length(),
                &stmt,
                nullptr
                ) != SQLITE_OK
            )
        throw std::runtime_error(
                mkstr() << "preparing SQLite DELETE query \"" <<
                    oss.str() << "\" message: " <<
                    sqlite3_errmsg(conn.handle())
                );
    auto id = static_cast<Tuple&>(*this).id();
    Tuple::id_type::bind_key_values(id, stmt);
    if(sqlite3_step(stmt) != SQLITE_DONE)
        throw std::runtime_error(
                mkstr() << "stepping SQLite DELETE query \"" <<
                    oss.str() << "\" message: " <<
                    sqlite3_errmsg(conn.handle())
                );
    int count = sqlite3_changes(conn.handle());
    int finalise_ret = sqlite3_finalize(stmt);
    if(finalise_ret != SQLITE_OK && finalise_ret != SQLITE_DONE)
        throw std::runtime_error(
                mkstr() << "finalising SQLite DELETE query \"" <<
                    oss.str() << "\" message: " <<
                    sqlite3_errmsg(conn.handle())
                );
    return (count > 0);
}

#endif

