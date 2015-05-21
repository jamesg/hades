#ifndef HADES_CRUD_SAVE_COLLECTION_IPP
#define HADES_CRUD_SAVE_COLLECTION_IPP

#include "hades/crud.hpp"

template<typename Tuple>
void hades::crud<Tuple>::save_collection(
        const styx::list& list,
        connection& conn
        )
{
    for(const styx::element& e : list)
    {
        Tuple t(e);
        t.save(conn);
    }
}

#endif

