#ifndef HADES_CRUD_HPP
#define HADES_CRUD_HPP

#include "hades/crud/destroy.ipp"
#include "hades/crud/insert.ipp"
#include "hades/crud/save.ipp"
#include "hades/crud/save_flags.ipp"
#include "hades/crud/update.ipp"
#include "hades/detail/basic_has_flags.hpp"
#include "hades/detail/last_insert_rowid.hpp"
#include "hades/flag.hpp"
#include "hades/get_collection.hpp"
#include "hades/transaction.hpp"
#include "hades/transaction.hpp"

namespace hades
{
    class connection;

    template<typename Tuple>
    class crud;

    /*!
     * \brief Provide basic CRUD (create, read, update and delete) functions
     * for a database type.  Functions provide syntactic sugar for free
     * functions in namespace hades (get_collection, insert, save, update).
     *
     * \param Tuple Database structure for which to provide CRUD functions.
     * Must derive from hades::has_candidate_key and hades::tuple.
     *
     * \example
     * class new_db_struct :
     *     hades::tuple<...>,
     *     hades::has_candidate_key<...>,
     *     hades::crud<new_db_struct>
     */
    template<typename Tuple>
    class crud
    {
    public:
        //
        // Permanent storage functions.
        //

        static styx::list get_collection(hades::connection& conn)
        {
            return hades::get_collection<Tuple>(conn);
        }

        /*!
         * \brief Insert the tuple into the database.  The id of this tuple
         * will be set to the id of the database tuple.
         *
         * \note This function was written to prevent
         * "tuple.set_id(hades::insert(...))" in external code.
         */
        void insert(hades::connection& conn)
        {
            detail::insert(static_cast<Tuple&>(*this), conn);
        }

        //
        // save_flags: there are two versions of this function, one for
        // tuple types deriving from hades::has_flags which saves all the
        // related flags, and one which does nothing.
        //

        /*!
         * \brief Save the tuple's associated flags into their relations.
         */
        //void save_flags(connection& conn)
        //{
            //Tuple& t = static_cast<Tuple&>(*this);
            //detail::save_flags<Tuple>(t, conn);
        //}

        /*!
         * \brief Save the tuple to the database.  Updates the tuple if it
         * already has an id, inserts the tuple otherwise.  The id of this
         * tuple will be set to the id of the database tuple.
         *
         * \note This function was written to prevent
         * "tuple.set_id(hades::save(...))" in external code.
         *
         * \returns True if a new record was created, false if an existing
         * record was updated.
         */
        bool save(connection& conn)
        {
            transaction tr(conn, "crud_save_transaction");
            detail::save(static_cast<Tuple&>(*this), conn);
            detail::save_flags<Tuple>(static_cast<Tuple&>(*this), conn);
            //Tuple::template save_flags<Tuple>(static_cast<Tuple&>(*this), conn);
            tr.commit();
        }

        /*!
         * \brief Updates the tuple in the database.  The id of this tuple
         * will be set to the id of the database tuple.
         *
         * \note This function was written to prevent
         * "tuple.set_id(hades::save(...))" in external code.
         */
        bool update(connection& conn)
        {
            detail::update(static_cast<Tuple&>(*this), conn);
        }

        bool destroy(connection& conn)
        {
            detail::destroy(static_cast<Tuple&>(*this), conn);
        }
    };
}

#endif

