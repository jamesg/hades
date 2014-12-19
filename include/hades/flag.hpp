#ifndef HADES_FLAG_HPP
#define HADES_FLAG_HPP

#include "styx/object.hpp"
#include "hades/crud.hpp"
#include "hades/relation.hpp"

namespace hades
{
    class connection;
    namespace detail
    {
        struct basic_flag
        {
        };
    }
    /*!
     * \brief Tuple type representing a binary flag.
     *
     * Flags are indicated by the presence or absence of a tuple in the relvar
     * named 'Relation', which has attributes matching the related tuple's
     * candidate key.
     */
    template<typename Id, const char *Relation>
    struct flag :
        public detail::basic_flag,
        public Id::candidate_key_type,
        public Id::candidate_key_type::tuple_type,
        public relation<Relation>,
        public crud<flag<Id, Relation>>
    {
        flag(Id id_) :
            styx::object(id_)
        {
        }
    };
}

#endif

