#ifndef HADES_ATTRIBUTE_LIST_HPP
#define HADES_ATTRIBUTE_LIST_HPP

#ifdef HADES_ENABLE_DEBUGGING
#include <iostream>
#endif
#include <type_traits>

#include "hades/bind_values.hpp"
#include "hades/get_column.hpp"
#include "styx/object.hpp"

namespace hades
{
    template<const char *...Attributes>
    class has_candidate_key;

    template<const char *...Attributes>
    class tuple;

    namespace detail
    {
        template<const char *Attr>
        struct attribute_list_member
        {
        };
    }

    /*!
     * \brief Meta class for operations on lists of attributes.
     *
     * Contains operations that are common to tuple, key and id types.
     */
    template<const char *...Attributes>
    class attribute_list :
        public detail::attribute_list_member<Attributes>...
    {
    public:
        /*!
         * \brief Extend this attribute list by appending one attribute to the
         * list of attributes.
         */
        template<const char *Attribute>
        using extend = typename std::conditional<
            std::is_base_of<
                detail::attribute_list_member<Attribute>,
                attribute_list<Attributes...>
                >::value,
            attribute_list<Attributes...>,
            attribute_list<Attributes..., Attribute>
            >::type;

        /*!
         * \brief A tuple type with the same attributes.
         */
        typedef tuple<Attributes...> tuple_type;

        /*!
         * \brief A has_candidate_key type with the same attributes.
         */
        typedef has_candidate_key<Attributes...> has_candidate_key_type;

        /*!
         * \brief Append a comma-separated list of columns to a stream.
         */
        static void column_list(std::ostream& os)
        {
            column_list_<Attributes...>(os);
        }

        /*!
         * \brief Append a comma-separated list of columns to a stream.  The
         * column names are qualified with the name of the relation provided as
         * the first template parameter.
         */
        template<const char *Relation>
        static void qualified_column_list(std::ostream& out)
        {
            qualified_column_list_<Relation, Attributes...>(out);
        }

        /*!
         * \return A vector containing attributes in Attributes as strings.
         */
        static std::vector<const char*> to_vector()
        {
            std::vector<const char*> out;
            to_vector_<Attributes...>(out);
            return out;
        }

        /*!
         * \brief Retrieve values from a SQLite statement.  Values are
         * retrieved in order.
         *
         * \param Start The index of the first value to retrieve.  The
         * first value in the row has index 0.
         *
         * \note As type information for the attributes of the tuple is
         * unknown at this point, attributes will be retrieved as strings.
         * SQLite will convert other types to strings as required.
         */
        template<int Start=0>
        static void retrieve_values(sqlite3_stmt *stmt, styx::object& out)
        {
            // 0 is the first index when retrieving values.
            retrieve_values_<Start, Attributes...>(stmt, out);
        }

        static void copy_attributes(
                styx::object& from,
                styx::object& to
                )
        {
            copy_attributes_<Attributes...>(from, to);
        }

        template<const char *Relation1, const char *Relation2>
        static void equijoin_on_clause(std::ostream& out)
        {
            equijoin_on_clause_<Relation1, Relation2, Attributes...>(out);
        }

        static bool all_attributes_set(styx::object& obj)
        {
            return all_attributes_set_<Attributes...>(obj);
        }

        static bool less_than(
                const styx::object& x,
                const styx::object& y
                )
        {
            return less_than_<Attributes...>(x, y);
        }

        static bool equal(
                const styx::object& x,
                const styx::object& y
                )
        {
            return equal_<Attributes...>(x, y);
        }

        static bool not_equal(
                const styx::object& x,
                const styx::object& y
                )
        {
            return not_equal_<Attributes...>(x, y);
        }

    private:
        template<const char *Attr1, const char *Attr2, const char *...Attrs>
        static void column_list_(std::ostream& os)
        {
            column_list_<Attr1>(os);
            os << ", ";
            column_list_<Attr2, Attrs...>(os);
        }

        template<const char *Attr1>
        static void column_list_(std::ostream& os)
        {
            os << Attr1;
        }

        template<const char *Relation, const char *Attr1>
        static void qualified_column_list_(std::ostream& out)
        {
            out << Relation << "." << Attr1;
        }

        template<
            const char *Relation,
            const char *Attr1,
            const char *Attr2,
            const char *...Attrs>
        static void qualified_column_list_(std::ostream& out)
        {
            qualified_column_list_<Relation, Attr1>(out);
            out << ", ";
            qualified_column_list_<Relation, Attr2, Attrs...>(out);
        }

        template<const char *Attr1, const char *Attr2, const char *...Attrs>
        static void to_vector_(std::vector<const char*>& out)
        {
            out.push_back(Attr1);
            to_vector_<Attr2, Attrs...>(out);
        }

        template<const char *Attr>
        static void to_vector_(std::vector<const char*>& out)
        {
            out.push_back(Attr);
        }

        template<int Index, const char *Attr1, const char *Attr2, const char *...Attrs>
        static void retrieve_values_(sqlite3_stmt *stmt, styx::object& out)
        {
            retrieve_values_<Index, Attr1>(stmt, out);
            retrieve_values_<Index+1, Attr2, Attrs...>(stmt, out);
        }

        template<int Index, const char *Attr>
        static void retrieve_values_(sqlite3_stmt *stmt, styx::object& out)
        {
            std::string s;
            hades::get_column(stmt, Index, s);
#ifdef HADES_ENABLE_DEBUGGING
            std::jerr <<
                "hades::attribute_list::retrieve_values: retrieving " <<
                Index << " " << Attr << " value: " << s << std::endl;
#endif
            out.get_string(Attr) = s;
        }

        template<int Index>
        static void retrieve_values_(sqlite3_stmt *stmt, styx::object& out)
        {
        }

        template<const char *Attr1, const char *Attr2, const char *...Attrs>
        static void copy_attributes_(
                styx::object& from,
                styx::object& to
                )
        {
            copy_attributes_<Attr1>(from, to);
            copy_attributes_<Attr2, Attrs...>(from, to);
        }

        template<const char *Attr>
        static void copy_attributes_(
                styx::object& from,
                styx::object& to
                )
        {
            to.get_element(Attr) = from.get_element(Attr);
        }

        template<
            const char *Relation1,
            const char *Relation2,
            const char *Attr1,
            const char *Attr2,
            const char *...Attrs>
        static void equijoin_on_clause_(std::ostream& out)
        {
            equijoin_on_clause_<Relation1, Relation2, Attr1>(out);
            out << " AND ";
            equijoin_on_clause_<Relation1, Relation2, Attr2, Attrs...>(out);
        }

        template<const char *Relation1, const char *Relation2, const char *Attr>
        static void equijoin_on_clause_(std::ostream& out)
        {
            out << Relation1 << "." << Attr << " = " << Relation2 << "." << Attr;
        }

        template<const char *Attr1>
        static bool all_attributes_set_(styx::object& obj)
        {
            return obj.has_key(Attr1);
        }

        template<const char *Attr1, const char *Attr2, const char *...Attrs>
        static bool all_attributes_set_(styx::object& obj)
        {
            return all_attributes_set_<Attr1>(obj) &&
                all_attributes_set_<Attr2, Attrs...>(obj);
        }

        template<const char *Attr1>
        static bool less_than_(
                const styx::object& x,
                const styx::object& y
                )
        {
            return x.copy_string(Attr1) < y.copy_string(Attr1);
        }

        template<const char *Attr1, const char *Attr2, const char *...Attrs>
        static bool less_than_(
                const styx::object& x,
                const styx::object& y
                )
        {
            if(less_than_<Attr1>(x, y))
                return true;
            return less_than_<Attr2, Attrs...>(x, y);
        }

        template<const char *Attr1>
        static bool equal_(
                const styx::object& x,
                const styx::object& y
                )
        {
            return x.copy_string(Attr1) == y.copy_string(Attr1);
        }

        template<const char *Attr1, const char *Attr2, const char *...Attrs>
        static bool equal_(
                const styx::object& x,
                const styx::object& y
                )
        {
            if(equal_<Attr1>(x, y))
                return true;
            return equal_<Attr2, Attrs...>(x, y);
        }

        template<const char *Attr1>
        static bool not_equal_(
                const styx::object& x,
                const styx::object& y
                )
        {
            return x.copy_string(Attr1) != y.copy_string(Attr1);
        }

        template<const char *Attr1, const char *Attr2, const char *...Attrs>
        static bool not_equal_(
                const styx::object& x,
                const styx::object& y
                )
        {
            if(not_equal_<Attr1>(x, y))
                return true;
            return not_equal_<Attr2, Attrs...>(x, y);
        }
    };

    template<>
    void attribute_list<>::column_list(std::ostream&);
    template<>
    void attribute_list<>::copy_attributes(
        styx::object&,
        styx::object&
        );
    template<>
    std::vector<const char*> attribute_list<>::to_vector();
    template<>
    bool attribute_list<>::all_attributes_set(styx::object& obj);
}

#endif

