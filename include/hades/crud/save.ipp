#ifndef HADES_CRUD_SAVE_IPP
#define HADES_CRUD_SAVE_IPP

#include "hades/crud/insert.ipp"
#include "hades/crud/update.ipp"
#include "hades/transaction.hpp"

namespace hades
{
    class connection;
    namespace detail
    {
        /*!
         * \brief Save the tuple to the database.  Updates the tuple if it
         * already has an id, inserts the tuple otherwise.  The id of this
         * tuple will be set to the id of the database tuple.
         *
         * \returns True if a new record was created, false if an existing
         * record was updated.
         */
        template<typename Tuple>
        bool save(Tuple& t, connection& conn)
        {
            transaction tr(conn, "save_transaction");
            bool updated = false;
            try
            {
                updated = update(t, conn);
            }
            catch(const std::exception& e)
            {
                // An exception can be thrown if the update fails.
            }
            if(!updated)
                insert(t, conn);
            //detail::save_flags<Tuple>(*this, conn);
            tr.commit();
            return !updated;
        }
    }
}

#endif

