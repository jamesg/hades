#ifndef HADES_HAS_KEY_HPP
#define HADES_HAS_KEY_HPP

#include <type_traits>

#include "styx/object_accessor.hpp"

namespace hades
{
    namespace detail
    {
        template<const char *Attribute>
        class has_key_attr;

        template<typename DbType, const char *Attribute>
        typename std::enable_if<std::is_base_of<has_key_attr<Attribute>, DbType>::value, int&>::type
        get_key(DbType& db)
        {
            return db.get_int(Attribute);
        }

        /*!
         * Give a DB type one key attribute.  Multiple keys can be added to
         * a type to form a candidate key.
         */
        template<const char *Attribute>
        class has_key_attr :
            public virtual styx::object_accessor
        {
            public:
                has_key_attr()
                {
                }

                explicit has_key_attr(const styx::element& o) :
                    styx::object_accessor(o)
                {
                }
        };
    }
}

#endif

