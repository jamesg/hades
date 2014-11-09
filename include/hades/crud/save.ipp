#ifndef HADES_CRUD_SAVE_IPP
#define HADES_CRUD_SAVE_IPP

#include "hades/transaction.hpp"

namespace hades
{
    class connection;
}

template<typename Tuple>
bool hades::crud<Tuple>::save(connection& conn)
{
            //detail::save(static_cast<Tuple&>(*this), conn);
    transaction tr(conn, "crud_save_transaction");
    bool updated = false;
    try
    {
        updated = update(conn);
    }
    catch(const std::exception& e)
    {
        // An exception can be thrown if the update fails.
    }
    if(!updated)
        insert(conn);
    tr.commit();
    return !updated;
}

#endif

