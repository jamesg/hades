#ifndef HADES_CRUD_FROM_ID_IPP
#define HADES_CRUD_FROM_ID_IPP

#include "hades/crud.hpp"
#include "hades/get_by_id.hpp"

template<typename Tuple>
template<typename Id>
void hades::crud<Tuple>::from_id(
        connection& conn,
        Id id
        )
{
    static_cast<Tuple&>(*this) = get_by_id<Tuple>(conn, id);
}

#endif

