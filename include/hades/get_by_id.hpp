#ifndef HADES_GET_BY_ID_HPP
#define HADES_GET_BY_ID_HPP

#ifdef HADES_ENABLE_DEBUGGING
#include <iostream>
#endif
#include <sqlite3.h>

#include "styx/serialisers/vector.hpp"

#include "hades/bind_values.hpp"
#include "hades/compound_id.hpp"
#include "hades/connection.hpp"
#include "hades/crud/get_flags.ipp"
#include "hades/exception.hpp"
#include "hades/get_one.hpp"
#include "hades/mkstr.hpp"
#include "hades/retrieve_values.hpp"
#include "hades/transaction.hpp"

namespace hades
{
    /*!
     * \brief Get a tuple from the database with every attribute in the
     * candidate key matching the attribute in 'id'.
     *
     * \param Out Database struct inheriting from has_candidate_key, tuple
     * and relation.
     * \param id Compound id (instance of a candidate key) to select from
     * the database relation.
     */
    template<typename Out>
    Out get_by_id(
            connection& conn,
            typename Out::id_type id
            )
    {
        hades::transaction transaction(conn, "hades_get_by_id");
        Out out = hades::get_one<Out>(
                conn,
                id.where()
                );
        detail::get_flags<Out>(conn, out);
        return out;
    }
}

#endif

