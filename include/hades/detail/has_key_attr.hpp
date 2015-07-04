#ifndef HADES_HAS_KEY_HPP
#define HADES_HAS_KEY_HPP

#include <type_traits>

#include "styx/object.hpp"

namespace hades
{
    namespace detail
    {
        template<const char *Attribute>
        class has_key_attr;

        template<typename DbType, const char *Attribute>
        typename std::enable_if<std::is_base_of<has_key_attr<Attribute>, DbType>::value, styx::int_type&>::type
        get_key(DbType& db)
        {
            return db.get_int(Attribute);
            //return 1;
        }

        /*!
         * Give a DB type one key attribute.  Multiple keys can be added to
         * a type to form a candidate key.
         */
        template<const char *Attribute>
        class has_key_attr
        {
        };
    }
}

#endif

