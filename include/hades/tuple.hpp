#ifndef HADES_TUPLE_HPP
#define HADES_TUPLE_HPP

#include <type_traits>

#include "styx/element.hpp"
#include "styx/object_accessor.hpp"

#include "hades/attribute_list.hpp"

namespace hades
{
    template<const char *...Attributes>
    class has_candidate_key;

    namespace detail
    {
        /*!
         * \internal
         * \brief Establish the fact that a type has an attribute.  Used to
         * enable accessor functions when the type has the required attribute.
         */
        template<const char *Attribute>
        struct has_attribute
        {
        };

        /*!
         * Get the attribute named 'Attribute' from a tuple.  Will only be
         * instantiated if the tuple type inherits from
         * has_attribute<Attribute>.
         */
        template<typename TupleType, const char *Attribute, typename Out>
        typename std::enable_if<
            std::is_base_of<has_attribute<Attribute>, TupleType>::value,
            Out&
            >::type
        get_attribute(TupleType& tuple)
        {
            return tuple.template get<Out>(Attribute);
        }
    }

    /*!
     * \brief A tuple (mapping of attribute name to attribute value) that
     * can be stored in a relational database.
     *
     * \note This class should be extended (as well as virtual
     * styx::object_accessor) to create accessor types for JSON objects that
     * can be stored in the database.
     */
    template<const char *...Attributes>
    class tuple :
        private detail::has_attribute<Attributes>...,
        public virtual styx::object_accessor
    {
        public:
            tuple()
            {
            }
            tuple(const styx::element& e) :
                styx::object_accessor(e)
            {
            }
            /*!
             * \brief Extend this tuple type by appending one attribute to the
             * list of attributes.
             * \deprecated
             */
            template<const char *Attribute>
            using extend = tuple<Attributes..., Attribute>;
            /*!
             * \brief A hades::attribute_list with the same attributes.
             */
            typedef attribute_list<Attributes...> attribute_list_type;

            static constexpr const size_t arity = sizeof...(Attributes);

            //
            // Alternatives to styx::object_accessor's get_ functions,
            // accepting the key as a template argument rather than a function
            // argument.  They offer no performance benefit; the only benefit
            // is that requests for invalid attributes can be made a
            // compile-time error.
            //

            template<const char *Attr>
            bool& get_bool()
            {
                return get_attr<Attr, bool&>();
            }
            template<const char *Attr>
            double& get_double()
            {
                return get_attr<Attr, double&>();
            }
            template<const char *Attr>
            int& get_int()
            {
                return get_attr<Attr, int&>();
            }
            template<const char *Attr>
            std::string& get_string()
            {
                return get_attr<Attr, std::string&>();
            }
            //template<const char *Attr>
            //bool has_key()
            //{
                //return has_key(Attr);
            //}

            /*!
             * \return A vector containing attributes which have values (are not styx::null_t).
             */
            std::vector<const char*> value_attributes()
            {
                std::vector<const char*> out;
                value_attributes_<Attributes...>(out);
                return out;
            }

            /*!
             * \brief Bind the values in the tuple, in order, to the SQLite
             * statement.  Null will be bound to attributes that do not have a
             * value.
             *
             * \param Start The index of the first placeholder to bind to.  The
             * first placeholder has index 1.
             */
            template<int Start=1>
            void bind_values(sqlite3_stmt *stmt)
            {
                // 1 is the first bound parameter index.
                bind_values_<Start, Attributes...>(stmt);
            }

            /*!
             * \brief Bind the values in the tuple, in order, to the SQLite
             * statement.  Only attributes with values will be bound (so the
             * number of bound values may not match the number of attributes).
             * The placeholder indexes will still be consecutive.
             *
             * \param Start The index of the first placeholder to bind to.  The
             * first placeholder has index 1.
             */
            template<int Start=1>
            void bind_set_values(sqlite3_stmt *stmt)
            {
                // 1 is the first bound parameter index.
                bind_set_values_<Start, Attributes...>(stmt);
            }

            bool operator<(const tuple<Attributes...>& o) const
            {
                return attribute_list<Attributes...>::less_than(*this, o);
            }
            bool operator==(const tuple<Attributes...>& o) const
            {
                return attribute_list<Attributes...>::equal(*this, o);
            }
            bool operator!=(const tuple<Attributes...>& o) const
            {
                return attribute_list<Attributes...>::not_equal(*this, o);
            }

        private:
            /*!
             * \brief Get a reference to one of the attributes in the
             * tuple.
             *
             * \note This function can only be instantiated if Attr is a
             * member of Attributes.
             */
            template<const char *Attr, typename Out>
            Out get_attr()
            {
                return detail::get_attribute<
                    tuple<Attributes...>,
                    Attr,
                    Out>(*this);
            }

            template<const char *Attr1, const char *Attr2, const char *...Attrs>
            void value_attributes_(std::vector<const char*>& out)
            {
                value_attributes_<Attr1>(out);
                value_attributes_<Attr2, Attrs...>(out);
            }

            template<const char *Attr>
            void value_attributes_(std::vector<const char*>& out)
            {
                if(has_key(Attr))
                    out.push_back(Attr);
            }

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

            template<int Index, const char *Attr1, const char *Attr2, const char *...Attrs>
            void bind_set_values_(sqlite3_stmt *stmt)
            {
                if(has_key(Attr1))
                {
                    bind_set_values_<Index, Attr1>(stmt);
                    bind_set_values_<Index+1, Attr2, Attrs...>(stmt);
                }
                else
                    bind_set_values_<Index, Attr2, Attrs...>(stmt);
            }

            template<int Index, const char *Attr>
            void bind_set_values_(sqlite3_stmt *stmt)
            {
                if(has_key(Attr))
                    hades::bind_any(Index, get<styx::element>(Attr), stmt);
            }
    };
}

#endif

