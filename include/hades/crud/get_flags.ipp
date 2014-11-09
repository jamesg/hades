#ifndef HADES_CRUD_GET_FLAGS_IPP
#define HADES_CRUD_GET_FLAGS_IPP

#include <type_traits>

#include "hades/detail/basic_has_flags.hpp"

namespace hades
{
    class connection;
    namespace detail
    {
        template<typename Tuple>
        typename std::enable_if<std::is_base_of<basic_has_flags, Tuple>::value>::type
        get_flags(connection& conn, Tuple& t)
        {
            t.get_flags(conn, t);
        }

        template<typename Tuple>
        typename std::enable_if<!std::is_base_of<basic_has_flags, Tuple>::value>::type
        get_flags(connection&, Tuple&)
        {
            // There are no flags to get, do nothing.
        }
    }
}

#endif

