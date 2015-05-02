#ifndef HADES_CRUD_GET_COLLECTION_IPP
#define HADES_CRUD_GET_COLLECTION_IPP

#include "hades/crud.hpp"
#include "hades/get_collection.hpp"

template<typename Tuple>
styx::list hades::crud<Tuple>::get_collection(connection& conn)
{
    return hades::get_collection<Tuple>(conn);
}

template<typename Tuple>
styx::list hades::crud<Tuple>::get_collection(
        connection& conn,
        const basic_filter& filter
        )
{
    return hades::get_collection<Tuple>(conn, filter);
}

#endif

