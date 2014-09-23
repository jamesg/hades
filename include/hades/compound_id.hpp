#ifndef HADES_COMPOUND_ID_HPP
#define HADES_COMPOUND_ID_HPP

#include <utility>

#include "styx/element.hpp"
#include "styx/styx.hpp"
#include "styx/object_accessor.hpp"

#include "attribute_list.hpp"
#include "has_key.hpp"
#include "retrieve_values.hpp"

namespace hades
{
    namespace detail
    {
        /*!
         * \note Provides storage for the compound_id.
         */
        template<const char *...Keys>
        class compound_id_map :
            public detail::has_key<Keys>...,
            public attribute_list<Keys...>,
            public virtual styx::object_accessor
        {
        public:
            compound_id_map()
            {
            }

            compound_id_map(styx::element& o) :
                detail::has_key<Keys>(o)...,
                styx::object_accessor(o)
            {
            }

            styx::object_accessor& accessor()
            {
                return *this;
            }
        };

        /*!
         * \brief Initialise a compound_id_map (specialisation of
         * styx::object_accessor) with values.
         */
        template<
                     typename CompoundIdMap,
                     const char *Key1,
                     const char *Key2,
                     const char *...Keys
                     >
        void copy_id_list(
                std::initializer_list<int>::iterator i,
                std::initializer_list<int>::iterator end,
                CompoundIdMap& out
                )
        {
            if(i != end)
            {
                get_key<CompoundIdMap, Key1>(out) = *i;
                copy_id_list<CompoundIdMap, Key2, Keys...>(++i, end, out);
            }
        }

        /*!
         * \brief Initialise a compound_id_map (specialisation of
         * styx::object_accessor) with values.
         */
        template<typename CompoundIdMap, const char *Key1>
        void copy_id_list(
                std::initializer_list<int>::iterator i,
                std::initializer_list<int>::iterator end,
                CompoundIdMap& out
                )
        {
            if(i != end)
                get_key<CompoundIdMap, Key1>(out) = *i;
        }
    }

    /*!
     * \note This class provides its own storage.  To describe a database type
     * that has a candidate key use has_candidate_key instead.
     */
    template<const char *...Keys>
    class compound_id
    {
    public:
        typedef detail::compound_id_map<Keys...> id_map_type;

        static constexpr const size_t arity = sizeof...(Keys);

        template<int Start=0>
        static compound_id<Keys...> from_stmt(sqlite3_stmt *stmt)
        {
            detail::compound_id_map<Keys...> id;
            // 0 is the first index when retrieving values.
            attribute_list<Keys...>::template retrieve_values<Start>(
                    stmt,
                    id.accessor()
                    );
            return compound_id<Keys...>(id);
        }

        compound_id()
        {
        }
        compound_id(std::initializer_list<int> values)
        {
            detail::copy_id_list<detail::compound_id_map<Keys...>, Keys...>(
                    values.begin(),
                    values.end(),
                    m_map
                    );
        }
        compound_id(styx::element e) :
            m_map(e)
        {
        }
        compound_id(const compound_id<Keys...>& o) :
            m_map(o.m_map)
        {
        }
        compound_id(const detail::compound_id_map<Keys...>& map) :
            m_map(map)
        {
        }

        static void copy_attributes_from_id_map(
                detail::compound_id_map<Keys...> from,
                styx::object_accessor& to
                )
        {
            copy_attributes_from_id_map_<Keys...>(from, to);
        }

        template<const char *Key_>
        int& key()
        {
            return detail::get_key<detail::compound_id_map<Keys...>, Key_>(
                    m_map
                    );
        }
        detail::compound_id_map<Keys...>& id_map()
        {
            return m_map;
        }
        /*!
         * \brief Bind values of a compound_id to a statement.
         */
        template<int Start=1>
        static void bind_key_values(
                compound_id<Keys...>& id,
                sqlite3_stmt *stmt
                )
        {
            // 1 is the first placeholder index.
            bind_key_value_<Start, Keys...>(id, stmt);
        }
        /*!
         * \brief Output a where clause with question mark placeholders.
         */
        static void key_where_clause(std::ostream& os)
        {
            key_where_clause_<Keys...>(os);
        }

        /*!
         * \brief Output a comma-separated list of columns to the output
         * stream.
         */
        static void key_column_list(std::ostream& os)
        {
            attribute_list<Keys...>::column_list(os);
        }

        bool operator==(const compound_id<Keys...>& o) const
        {
            compound_id<Keys...> x = *this, y = o;
            return equal_<Keys...>(x.id_map(), y.id_map());
        }

        bool operator!=(const compound_id<Keys...>& o) const
        {
            compound_id<Keys...> x = *this, y = o;
            return !equal_<Keys...>(x.id_map(), y.id_map());
        }
    private:
        template<
            int Index,
            const char *Attr1,
            const char *Attr2,
            const char *...Attrs>
        static void bind_key_value_(
                compound_id<Keys...> id,
                sqlite3_stmt *stmt
                )
        {
            // Bind the current value.
            bind_key_value_<Index, Attr1>(id, stmt);
            // Bind following values.
            bind_key_value_<Index+1, Attr2, Attrs...>(id, stmt);
        }

        template<int Index>
        static void bind_key_value_(
                compound_id<Keys...>&,
                sqlite3_stmt*
                )
        {
        }

        template<int Index, const char *Attr1>
        static void bind_key_value_(
                compound_id<Keys...>& id,
                sqlite3_stmt *stmt
                )
        {
            hades::bind(
                Index,
                detail::get_key<id_map_type, Attr1>(id.id_map()),
                stmt
                );
        }

        template<const char *Attr1, const char *Attr2, const char *...Attrs>
        static void key_where_clause_(std::ostream& os)
        {
            key_where_clause_<Attr1>(os);
            os << " AND ";
            key_where_clause_<Attr2, Attrs...>(os);
        }

        template<const char *Attr1>
        static void key_where_clause_(std::ostream& os)
        {
            os << Attr1 << " = ? ";
        }

        template<const char *Attr1, const char *Attr2, const char *...Attrs>
        static void copy_attributes_from_id_map_(
                detail::compound_id_map<Keys...> from,
                styx::object_accessor& to
                )
        {
            copy_attributes_from_id_map_<Attr1>(from, to);
            copy_attributes_from_id_map_<Attr2, Attrs...>(from, to);
        }

        template<const char *Attr>
        static void copy_attributes_from_id_map_(
                detail::compound_id_map<Keys...> from,
                styx::object_accessor& to
                )
        {
            to.get_element(Attr) = from.get_element(Attr);
        }

        template<const char *Attr1, const char *Attr2, const char *...Attrs>
        static bool equal_(
                detail::compound_id_map<Keys...> x,
                detail::compound_id_map<Keys...> y
                )
        {
            return equal_<Attr1>(x, y) && equal_<Attr2, Attrs...>(x, y);
        }

        template<const char *Attr1>
        static bool equal_(
                detail::compound_id_map<Keys...> x,
                detail::compound_id_map<Keys...> y
                )
        {
            // TODO: Don't convert to strings if ids are of the same type to
            // start.
            styx::element x_elem = std::string(), y_elem = std::string();
            return
                styx::convert(x.get_element(Attr1), x_elem) &&
                styx::convert(y.get_element(Attr1), y_elem) &&
                boost::get<std::string>(x_elem) == boost::get<std::string>(y_elem);
        }

        detail::compound_id_map<Keys...> m_map;
    };

    template<>
    void compound_id<>::key_where_clause(std::ostream& os);

    template<>
    void compound_id<>::copy_attributes_from_id_map(
            detail::compound_id_map<>,
            styx::object_accessor&
            );
}

#endif

