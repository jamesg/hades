#ifndef HADES_CRUD_OVERWRITE_COLLECTION_IPP
#define HADES_CRUD_OVERWRITE_COLLECTION_IPP

#include "hades/crud.hpp"
#include "hades/transaction.hpp"

template<typename Tuple>
void hades::crud<Tuple>::overwrite_collection(
        const styx::list& list,
        connection& conn
        )
{
    hades::transaction transaction(conn, "hades_crud_overwrite_collection");
    Tuple::destroy_collection(conn);
    for(const styx::element& e : list)
    {
        Tuple t(e);
        t.insert(conn);
    }
    transaction.commit();
}

template<typename Tuple>
void hades::crud<Tuple>::overwrite_collection(
        const styx::list& list,
        const basic_filter& filter,
        connection& conn
        )
{
    hades::transaction transaction(conn, "hades_crud_overwrite_collection");
    Tuple::destroy_collection(filter, conn);
    for(const styx::element& e : list)
    {
        Tuple t(e);
        t.insert(conn);
    }
    transaction.commit();
}

#endif

