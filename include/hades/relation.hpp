#ifndef HADES_RELATION_HPP
#define HADES_RELATION_HPP

namespace hades
{
    /*!
     * \brief Establish a connection between a database struct and its
     * database relation or relvar (SQL table or base table).
     */
    template<const char *const Relation>
    struct relation
    {
        static constexpr const char * const relation_name = Relation;
    };
}

#endif

