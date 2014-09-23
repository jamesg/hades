#ifndef HADES_HAS_KEY_HPP
#define HADES_HAS_KEY_HPP

#include <type_traits>

#include "styx/object_accessor.hpp"

namespace hades
{
    namespace detail
    {
        template<const char *Attribute>
        class has_key;

        template<typename DbType, const char *Attribute>
        typename std::enable_if<std::is_base_of<has_key<Attribute>, DbType>::value, int&>::type
        get_key(DbType& db)
        {
            return db.get_int(Attribute);
        }

        /*!
         * Give a DB type one key attribute.  Multiple keys can be added to
         * a type to form a candidate key.
         */
        template<const char *Attribute>
        class has_key :
            public virtual styx::object_accessor
        {
            public:
                has_key()
                {
                }

                explicit has_key(const styx::element& o) :
                    styx::object_accessor(o)
                {
                }
        };
    }
}

#endif

