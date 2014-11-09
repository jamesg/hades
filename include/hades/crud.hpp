#ifndef HADES_CRUD_HPP
#define HADES_CRUD_HPP

#include "styx/list.hpp"

namespace hades
{
    class connection;
    class basic_filter;

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
        /*!
         * \brief Get all tuples of this type from the database.
         *
         * \note Does not obtain additional flags.
         */
        static styx::list get_collection(connection&);
        /*!
         * \brief Get all tuples of this type from the database matching a
         * filter.
         *
         * \note Does not obtain additional flags.
         */
        static styx::list get_collection(connection&, basic_filter&);

        /*!
         * \brief Retrieve all non-id fields of this tuple based on the id.
         *
         * \note Obtains additional flags.
         */
        template<typename Id>
        void from_id(connection&, Id id);

        /*!
         * \brief Check whether a tuple with the id of this tuple exists in the
         * database.
         */
        bool exists(connection&);

        /*!
         * \brief Insert the tuple into the database.  The id of this tuple
         * will be set to the id of the database tuple.
         *
         * \note This function was written to prevent
         * "tuple.set_id(hades::insert(...))" in external code.
         * \note Does not update additional flags.
         */
        void insert(connection&);

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
         * \note Updates additional flags.
         */
        bool save(connection&);

        /*!
         * \brief Updates the tuple in the database.  The id of this tuple
         * will be set to the id of the database tuple.
         *
         * \note This function was written to prevent
         * "tuple.set_id(hades::save(...))" in external code.
         * \note Updates additional flags.
         */
        bool update(connection&);

        /*!
         * \brief Delete the tuple from the database.
         *
         * \note Does not delete additional flags.  Flag tables should have
         * been created with "ON DELETE CASCADE".
         */
        bool destroy(connection&);
    };
}

#endif

