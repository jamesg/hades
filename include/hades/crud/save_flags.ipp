#ifndef HADES_CRUD_SAVE_FLAGS_IPP
#define HADES_CRUD_SAVE_FLAGS_IPP

#include <type_traits>

#include "hades/crud/save.ipp"
#include "hades/crud/insert.ipp"
#include "hades/crud/destroy.ipp"
#include "hades/detail/basic_has_flags.hpp"

namespace hades
{
    class connection;
    namespace detail
    {
        template<typename Tuple>
        typename std::enable_if<std::is_base_of<basic_has_flags, Tuple>::value>::type
        save_flags(Tuple& t, connection& conn)
        {
            t.save_flags(t, conn);
            //Tuple::template save_flags<Tuple>(static_cast<Tuple&>(*this), conn);
        }

        template<typename Tuple>
        typename std::enable_if<!std::is_base_of<basic_has_flags, Tuple>::value>::type
        save_flags(Tuple& t, connection& conn)
        {
            // There are no flags to save, do nothing.
        }
    }
}

#endif

