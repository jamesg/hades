#ifndef HADES_RELATIONS_ACCESSOR_HPP
#define HADES_RELATIONS_ACCESSOR_HPP

#include <type_traits>

#include "styx/object.hpp"
#include "styx/styx.hpp"

namespace hades
{
    namespace detail
    {
        template<const char *Attribute>
        class has_relation
        {
        };

        template<typename Accessor, const char *Attribute>
        typename std::enable_if<
            std::is_base_of<has_relation<Attribute>, Accessor>::value,
            styx::element&
            >::type
        get_element(styx::object& db)
        {
            return db.get_element(Attribute);
        }
    }

    /*!
     * \brief Accessor for a styx::element that contains several tuples.  The
     * key of each tuple is the relation name.
     */
    template<typename ...Relations>
    class relations_accessor :
        public detail::has_relation<(const char*)Relations::relation_name>...,
        public styx::object
    {
        public:
            relations_accessor()
            {
            }

            relations_accessor(styx::element& element) :
                styx::object(element)
            {
            }

            /*!
             * \brief Get the element in the Styx object representing the tuple
             * in the relation specified by the template parameter.
             */
            template<const char *Attribute>
            styx::element& get_sub_element()
            {
                return detail::get_element<
                    relations_accessor<Relations...>,
                    Attribute>(*this);
            }

            template<typename Tuple>
            Tuple get_tuple()
            {
                return Tuple(get_sub_element<(const char*)Tuple::relation_name>());
            }
    };
}

#endif

