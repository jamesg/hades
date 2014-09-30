#ifndef HADES_COMPOUND_ID_HPP
#define HADES_COMPOUND_ID_HPP

#include <utility>

#include "styx/element.hpp"
#include "styx/styx.hpp"
#include "styx/object_accessor.hpp"

#include "hades/attribute_list.hpp"
#include "hades/detail/has_key_attr.hpp"
#include "hades/retrieve_values.hpp"
#include "hades/tuple.hpp"

namespace hades
{
    namespace detail
    {
        /*!
         * \brief Initialise a styx::object_accessor with values from a
         * std::initializer_list.
         *
         * \param CompoundIdMap A styx::object_accessor derived type which also
         * inherits from detail::has_key_attr<Attr> for each attribute in the
         * Keys list.
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
                //get_key<CompoundIdMap, Key1>(out) = *i;
                out.template get_int<Key1>() = *i;
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
                out.template get_int<Key1>() = *i;
                //get_key<CompoundIdMap, Key1>(out) = *i;
        }
    }

    template<const char *...Attributes> class has_candidate_key;

    /*!
     * \note This class provides its own storage.  To describe a database type
     * that has a candidate key use has_candidate_key instead.
     */
    template<const char *...Keys>
    class compound_id :
        public hades::tuple<Keys...>/*,*/
        //public detail::has_key_attr<Keys>...
    {
    public:
        typedef has_candidate_key<Keys...> candidate_key_type;

        template<int Start=0>
        static compound_id<Keys...> from_stmt(sqlite3_stmt *stmt)
        {
            compound_id<Keys...> id;
            // 0 is the first index when retrieving values.
            attribute_list<Keys...>::template retrieve_values<Start>(
                    stmt,
                    id
                    );
            return id;
        }

        compound_id()
        {
        }
        compound_id(std::initializer_list<int> values)
        {
            detail::copy_id_list<compound_id<Keys...>, Keys...>(
                    values.begin(),
                    values.end(),
                    *this
                    );
        }
        compound_id(const styx::element& e) :
            styx::object_accessor(e)
        {
        }
        compound_id(const compound_id& m) :
            styx::object_accessor(m.get_element())
        {
        }

        //template<const char *Key_>
        //int& key()
        //{
            ////return detail::get_key<compound_id<Keys...>, Key_>(*this);
            //return tuple<Keys...>::template get_int<Key_>();
        //}
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
                id.template get_int<Attr1>(),/*(Attr1),*/
                //detail::get_key<compound_id<Keys...>, Attr1>(id),
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
    };

    template<>
    void compound_id<>::key_where_clause(std::ostream& os);
}

#endif

