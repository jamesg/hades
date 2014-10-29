#ifndef HADES_CRUD_EXISTS_HPP
#define HADES_CRUD_EXISTS_HPP

#ifdef HADES_ENABLE_DEBUGGING
#include <iostream>
#endif
#include <sqlite3.h>

#include "hades/connection.hpp"

template<typename Tuple>
bool hades::crud<Tuple>::exists(connection& conn)
{
    std::ostringstream oss;
    oss << "SELECT ";
    styx::serialise(
            Tuple::attribute_list_type::to_vector(),
            [](const char *s, std::ostream& os) {
                os << s;
            },
            ", ",
            oss
            );
    oss << " FROM " << Tuple::relation_name;
    oss << " WHERE ";
    Tuple::id_type::key_where_clause(oss);
#ifdef HADES_ENABLE_DEBUGGING
    std::cerr << "hades exists query: " << oss. str() << std::endl;
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
    {
        std::ostringstream oss_;
        oss_ << "error in SQLite select; query: \"" << oss.str() <<
            "\" sqlite error: " << sqlite3_errmsg(conn.handle()) << std::endl;
        throw std::runtime_error(oss_.str());
    }

    Tuple& t = static_cast<Tuple&>(*this);
    auto id = t.id();
    Tuple::id_type::bind_key_values(id, stmt);

    auto result = sqlite3_step(stmt);

    return (result == SQLITE_ROW);
}

#endif

