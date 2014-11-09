#ifndef HADES_CRUD_UPDATE_IPP
#define HADES_CRUD_UPDATE_IPP

#ifdef HADES_ENABLE_DEBUGGING
#include <iostream>
#endif
#include <sstream>
#include <sqlite3.h>

#include "hades/connection.hpp"
#include "hades/crud.hpp"
#include "hades/mkstr.hpp"
#include "styx/serialisers/vector.hpp"

template<typename Tuple>
bool hades::crud<Tuple>::update(connection& conn)
{
    Tuple& t = static_cast<Tuple&>(*this);
    if(!t.id_set())
        throw std::runtime_error("cannot update a tuple if id is incomplete");

    std::ostringstream oss;
    oss << "UPDATE " << Tuple::relation_name << " SET ";
    styx::serialise(
            Tuple::attribute_list_type::to_vector(),
            [](const char *s, std::ostream& os) {
                os << s << " = ? ";
            },
            ", ",
            oss
            );
    oss << " WHERE ";

    Tuple::id_type::key_where_clause(oss);

#ifdef HADES_ENABLE_DEBUGGING
        std::cerr << "hades update query: " << oss.str() << std::endl;
#endif

    sqlite3_stmt *stmt = nullptr;
    if(
            sqlite3_prepare(
                conn.handle(),
                oss.str().c_str(),
                -1,
                &stmt,
                nullptr
                ) != SQLITE_OK
            )
    {
        throw std::runtime_error(
                mkstr() << "hades::update: preparing UPDATE query \"" <<
                    oss.str() << "\": " << sqlite3_errmsg(conn.handle())
                );
    }

    t.bind_values(stmt);
    t.template bind_key_values<Tuple::arity+1>(stmt);

    int step_ret = sqlite3_step(stmt);
    if(step_ret != SQLITE_OK && step_ret != SQLITE_DONE)
    {
        throw std::runtime_error(
                mkstr() << "stepping SQLite UPDATE query \"" <<
                    oss.str() << "\": " << sqlite3_errmsg(conn.handle())
                );
    }

    int count = sqlite3_changes(conn.handle());

    int finalise_ret = sqlite3_finalize(stmt);
    if(finalise_ret != SQLITE_OK && finalise_ret != SQLITE_DONE)
    {
        throw std::runtime_error(
                mkstr() << "finalising SQLite UPDATE query \"" <<
                    oss.str() << "\": " << sqlite3_errmsg(conn.handle())
                );
    }

    // Only update the flags if a row was affected by the tuple update.
    if(count > 0)
        detail::save_flags<Tuple>(static_cast<Tuple&>(*this), conn);
    // The number of rows updated (0 or 1, because the target tuples were
    // selected by a candidate key).
    return (count > 0);
}

#endif

