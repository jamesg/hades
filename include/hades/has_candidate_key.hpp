#ifndef HADES_HAS_CANDIDATE_KEY_HPP
#define HADES_HAS_CANDIDATE_KEY_HPP

#ifdef HADES_ENABLE_DEBUGGING
#include <iostream>
#endif
#include <type_traits>

#include "styx/object.hpp"
#ifdef HADES_ENABLE_DEBUGGING
#include "styx/serialise_json.hpp"
#endif

#include "hades/compound_id.hpp"
#include "hades/detail/has_key_attr.hpp"
#include "hades/row.hpp"

namespace hades
{
    template<const char *...Attributes>
    class tuple;

    template<const char *...Attributes>
    class attribute_list;

    template<const char *Attr1, const char *...Attrs>
    struct first
    {
        static constexpr const char *value = Attr1;
    };

    /*!
     * \brief Establish a list of attributes as a candidate key for a type.
     */
    template<const char *...Attributes>
    class has_candidate_key :
        public virtual detail::has_key_attr<Attributes>...,
        public virtual styx::object
    {
        public:
            /*!
             * \brief The type used for IDs in the relation for which this key
             * is a candidate key.  An ID is an instance of a key.
             */
            typedef compound_id<Attributes...> id_type;
            /*!
             * \brief A hades::tuple with the same attributes.
             * \deprecated
             */
            typedef tuple<Attributes...> tuple_type;
            /*!
             * \brief A hades::attribute_list with the same attributes.
             */
            typedef attribute_list<Attributes...> key_attribute_list;
            typedef has_candidate_key<Attributes...> candidate_key_type;

            static constexpr const char *first_key_attr = first<Attributes...>::value;

            /*
             * Override the move constructors and assignment operators.
             *
             * Doing this will stop the operators in styx::object being called
             * twice when a tuple class inherits from virtual styx::object via
             * hades::has_candidate_key and hades::tuple.
             */

            has_candidate_key<Attributes...>()
            {
            }

            has_candidate_key<Attributes...>(const has_candidate_key<Attributes...>&)
            {
            }

            has_candidate_key<Attributes...>(has_candidate_key<Attributes...>&&)
            {
            }

            has_candidate_key<Attributes...>& operator=(const has_candidate_key<Attributes...>&)
            {
                return *this;
            }

            has_candidate_key<Attributes...>& operator=(has_candidate_key<Attributes...>&&)
            {
                return *this;
            }

            /*!
             * \brief Get the value of an attribute within the candidate
             * key.
             */
            template<const char *Attribute_>
            styx::int_type& key()
            {
                return detail::get_key<has_candidate_key<Attributes...>, Attribute_>(*this);
            }

            id_type id()
            {
                styx::element el = *this;//get_element();
                return id_type(el);
            }

            void set_id(compound_id<Attributes...> key)
            {
                attribute_list<Attributes...>::copy_attributes(key, *this);
            }

            template<int Start=1>
            void bind_key_values(sqlite3_stmt *stmt)
            {
                bind_values_<Start, Attributes...>(stmt);
            }

            bool id_set()
            {
                return attribute_list<Attributes...>::all_attributes_set(*this);
            }
        private:
            template<int Index, const char *Attr1, const char *Attr2, const char *...Attrs>
            void bind_values_(sqlite3_stmt *stmt)
            {
                bind_values_<Index, Attr1>(stmt);
                bind_values_<Index+1, Attr2, Attrs...>(stmt);
            }

            template<int Index, const char *Attr>
            void bind_values_(sqlite3_stmt *stmt)
            {
                hades::bind_any(Index, get<styx::element>(Attr), stmt);
            }

            template<int Index>
            void bind_values_(sqlite3_stmt*)
            {
            }
    };
}

#endif

