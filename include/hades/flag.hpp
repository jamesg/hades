#ifndef HADES_FLAG_HPP
#define HADES_FLAG_HPP

#include "styx/object_accessor.hpp"
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
    template<typename Id, const char *Relation>
    struct flag :
        public detail::basic_flag,
        public Id::candidate_key_type,
        public Id::candidate_key_type::tuple_type,
        public relation<Relation>,
        public crud<flag<Id, Relation>>
    {
        flag(Id id_) :
            styx::object_accessor(id_.get_element())
        {
        }
    };
}

#endif

