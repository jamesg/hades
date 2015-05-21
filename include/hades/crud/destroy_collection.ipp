#ifndef HADES_CRUD_DESTROY_COLLECTION_IPP
#define HADES_CRUD_DESTROY_COLLECTION_IPP

#include <sstream>

#include "hades/devoid.hpp"
#include "hades/filter.hpp"

template<typename Tuple>
void hades::crud<Tuple>::destroy_collection(connection& conn)
{
    std::ostringstream query;
    query << "DELETE FROM " << Tuple::relation_name;
    hades::devoid(query.str(), conn);
}

template<typename Tuple>
void hades::crud<Tuple>::destroy_collection(
        const hades::basic_filter& filter,
        connection& conn
        )
{
    std::ostringstream query;
    query << "DELETE FROM " << Tuple::relation_name << " " << filter.clause();
    sqlite3_stmt *stmt = nullptr;
    if(
        sqlite3_prepare(
            conn.handle(),
            query.str().c_str(),
            -1,
            &stmt,
            nullptr
            ) != SQLITE_OK
        )
    {
        throw std::runtime_error(
                mkstr() << "preparing SQLite statement \"" <<
                    query << "\""
                );
    }
    filter.bind(stmt);

    auto step = sqlite3_step(stmt);
#ifdef HADES_ENABLE_DEBUGGING
    std::cerr << "hades::crud::destroy_collection: query \"" << query.str() <<
        "\", exists: " << std::boolalpha << (step == SQLITE_ROW) <<
        std::endl;
#endif
    sqlite3_finalize(stmt);
}

#endif

