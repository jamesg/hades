#ifndef HADES_FLAGS_HPP
#define HADES_FLAGS_HPP

#include "hades/detail/has_attribute.hpp"

namespace hades
{
    namespace detail
    {
        template<const char *Relation>
        struct has_flag :
            public has_attribute<Relation>
        {
        };
    }
    template<const char *...Key>
    struct flags :
        detail::has_flag<Key>...
    {
    };
}

#endif

