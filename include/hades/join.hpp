#ifndef HADES_JOIN_HPP
#define HADES_JOIN_HPP

#include <iostream>
#include <type_traits>

#include <sqlite3.h>

#include "hades/attribute_list.hpp"
#include "hades/connection.hpp"
#include "hades/flag.hpp"
#include "hades/filter.hpp"
#include "hades/mkstr.hpp"
#include "hades/relation.hpp"
#include "hades/row.hpp"
#include "hades/tuple.hpp"

namespace hades
{
    namespace detail
    {
        //
        // relation_list: write a comma-separated list of relation names to a
        // stream.
        //

        template<const char *Join, typename Relation>
        void relation_list(std::ostream& os)
        {
            os << Relation::relation_name;
        }

        template<const char *Join, typename Relation1, typename Relation2, typename ...Relations>
        void relation_list(std::ostream& os)
        {
            relation_list<Join, Relation1>(os);
            os << Join;
            relation_list<Join, Relation2, Relations...>(os);
        }

        //
        // on_clause: generate the 'ON t1.id = t2.id'... part of the query.
        //

        template<typename Relation1>
        void on_clause(std::ostream& os)
        {
            // No 'ON' clause for a single relation.
        }

        template<typename Relation1, typename Relation2>
        void on_clause(std::ostream& os)
        {
            static_assert(
                    std::is_same<
                        typename Relation1::candidate_key_type,
                        typename Relation2::candidate_key_type>::value,
                    "cannot equijoin relations with different key types"
                    );
            typedef typename Relation1::key_attribute_list attr_list;
            attr_list::template equijoin_on_clause<
                (const char*)Relation1::relation_name,
                (const char*)Relation2::relation_name
                >(os);
        }

        template<
            typename Relation1,
            typename Relation2,
            typename Relation3,
            typename ...Relations>
        void on_clause(std::ostream& os)
        {
            on_clause<Relation1, Relation2>(os);
            os << " AND ";
            on_clause<Relation2, Relation3, Relations...>(os);
        }

        //
        // all_column_list: write the attribute list part of the SELECT
        // statement.  Behaviour depends on the type of tuple.  For standard
        // tuples, list all attributes.  For flags, extend the result with a
        // boolean attribute indicating whether the flag is present or not.
        //

        //
        // First version: write a comma-separated list of extended columns of
        // the form '(t.id IS NULL) as t_exists'.  Used to recover boolean
        // flags from the result of a join.
        //
        template<typename Flag>
        typename std::enable_if<std::is_base_of<detail::basic_flag, Flag>::value>::type
        all_column_list(std::ostream& os)
        {
            os << "(" << Flag::relation_name << "." <<
                Flag::first_key_attr << " IS NULL) AS " <<
                Flag::relation_name << "_exists";
        }

        //
        // Second version: write a comma-separated list of column names from
        // all tuple types to an output stream.
        //
        template<typename Tuple>
        typename std::enable_if<!std::is_base_of<detail::basic_flag, Tuple>::value>::type
        all_column_list(std::ostream& os)
        {
            typedef typename Tuple::attribute_list_type attrl;
            attrl::template qualified_column_list<
                (const char*)Tuple::relation_name
                >(os);
        }

        template<typename Tuple1, typename Tuple2, typename ...Tuples>
        void all_column_list(std::ostream& os)
        {
            all_column_list<Tuple1>(os);
            os << ", ";
            all_column_list<Tuple2, Tuples...>(os);
        }

        //
        // retrieve_tuple_values: Retrieve values from a SQLite statement and
        // record them in an object_accessor.  Values are retrieved starting at
        // an index supplied as the first template parameter.
        //
        // Again, there are two versions.  The flag version gets the boolean
        // attribute indicating presence of the tuple.  The tuple version gets
        // all attributes from the tuple (using
        // attribute_list::retrieve_values).
        //

        template<int Start, typename Flag>
        typename std::enable_if<std::is_base_of<detail::basic_flag, Flag>::value>::type
        retrieve_tuple_values(
                sqlite3_stmt *stmt,
                styx::object_accessor& accessor
                )
        {
            int out = 0;
            get_column(stmt, Start, out);
            accessor.get_bool(Flag::relation_name) = (out > 0);
        }

        template<int Start, typename Tuple>
        typename std::enable_if<!std::is_base_of<detail::basic_flag, Tuple>::value>::type
        retrieve_tuple_values(
                sqlite3_stmt *stmt,
                styx::object_accessor& accessor
                )
        {
            typedef typename Tuple::attribute_list_type attrl;
            attrl::template retrieve_values<Start>(stmt, accessor);
        }

        //
        // Traits types to get the number of attributes in the result related
        // to a tuple in the join.  This is the number of attributes appended
        // by all_column_list<Tuple>.
        //

        struct arity_one
        {
            static constexpr const size_t value = 1;
        };
        template<typename Tuple>
        struct arity_copy
        {
            static constexpr const size_t value = Tuple::arity;
        };
        template<typename Tuple>
        struct arity
        {
            typedef typename std::conditional<
                std::is_base_of<detail::basic_flag, Tuple>::value,
                arity_one,
                arity_copy<Tuple>
                >::type arity_type;
            static constexpr const size_t value = arity_type::value;
        };

        //
        // join_values: Traits type to retrieve values from the result of a join
        // (a set of relations).
        //

        template<typename ...Relations>
        struct join_values
        {
            static void retrieve_join_values(
                    sqlite3_stmt *stmt,
                    styx::object_accessor& accessor
                    )
            {
                retrieve_join_values_<0, Relations...>(stmt, accessor);
            }
        private:
            template<int Start, typename Rel>
            static void retrieve_join_values_(
                    sqlite3_stmt *stmt,
                    styx::object_accessor& accessor
                    )
            {
                retrieve_tuple_values<Start, Rel>(stmt, accessor);
            }

            template<int Start, typename Rel1, typename Rel2, typename ...Rels>
            static void retrieve_join_values_(
                    sqlite3_stmt *stmt,
                    styx::object_accessor& accessor
                    )
            {
                retrieve_join_values_<Start, Rel1>(stmt, accessor);
                retrieve_join_values_<Start + arity<Rel1>::value, Rel2, Rels...>(
                        stmt,
                        accessor
                        );
            }
        };

        namespace join_type
        {
            extern const char inner[];
            extern const char outer[];
        }

        template<bool EquiJoin, typename ...Tuples>
        typename std::enable_if<EquiJoin, void>::type equijoin_on_clause(
                std::ostream& os
                )
        {
            os << " ON ";
            on_clause<Tuples...>(os);
        }

        template<bool EquiJoin, typename ...Tuples>
        typename std::enable_if<!EquiJoin, void>::type equijoin_on_clause(
                std::ostream& os
                )
        {
        }
    }

    /*!
     * \brief Execute an SQL SELECT query on multiple tables.  All attributes
     * of all relations used in the JOIN are retrieved.
     *
     * \internal
     *
     * \param Join Type of join (see namespace hades::detail::join_type).
     * \param Values A Boost Fusion container type.  hades::row is an easy to
     * use container.
     * \param Tuples A type inheriting from hades::tuple and hades::relation.
     * Determines the attributes and relation that will be queried.
     * \param values Boost Fusion container of values to bind to placeholders
     * in the query.
     * \param filter SQL filter clause to use for the query.  Question mark
     * placeholders are filled with values in 'values'.
     */
    template<const char *Join, bool EquiJoin, typename ...Tuples>
    styx::list join_(
            connection& conn,
            const basic_filter& filter_
            )
    {
        styx::list list;

        std::ostringstream query;
        query << "SELECT ";
        detail::all_column_list<Tuples...>(query);
        query << " FROM ";
        detail::relation_list<Join, Tuples...>(query);
        if(sizeof...(Tuples) > 1)
            detail::equijoin_on_clause<EquiJoin, Tuples...>(query);
        query << filter_.clause();

        sqlite3_stmt *stmt = nullptr;
        if(
            sqlite3_prepare(
                conn.handle(),
                query.str().c_str(),
                -1,
                &stmt,
                nullptr
                ) != SQLITE_OK
            )
        {
            throw std::runtime_error(
                    mkstr() << "preparing SQLite statement \"" <<
                        query.str() << "\" (" <<
                        sqlite3_errmsg(conn.handle()) << ")"
                    );
        }
        filter_.bind(stmt);

        while(sqlite3_step(stmt) == SQLITE_ROW)
        {
            try
            {
                styx::object_accessor accessor;
                detail::join_values<Tuples...>::retrieve_join_values(
                        stmt,
                        accessor
                        );
                list.append(accessor.get_element());
            }
            catch(const std::exception&)
            {
                sqlite3_finalize(stmt);
                throw;
            }
        }

        if(sqlite3_finalize(stmt) != SQLITE_OK)
            throw std::runtime_error("finalizing SQLite statement");

        return list;
    }

    /*!
     * \brief Execute an SQL SELECT query on multiple tables.  Tables are joined
     * using an INNER JOIN.  All attributes in all tables will be retrieved
     * using a hades::relations_accessor.
     *
     * \returns A styx::list of styx::objects.  Objects contain all keys from
     * all tuples in the join.
     */
    template<typename ...Tuple>
    styx::list join(connection& conn)
    {
        return join_<detail::join_type::inner, false, Tuple...>(
                conn,
                filter_all()
                );
    }

    /*!
     * \brief Execute an SQL SELECT query on multiple tables.  Tables are joined
     * using an INNER JOIN.  All attributes in all tables will be retrieved
     * using a hades::relations_accessor.
     *
     * \returns A styx::list of styx::objects.  Objects contain all keys from
     * all tuples in the join.
     */
    template<typename ...Tuple>
    styx::list join(
            connection& conn,
            const basic_filter& filter_
            )
    {
        return join_<detail::join_type::inner, false, Tuple...>(
                conn,
                filter_
                );
    }

    /*!
     * \brief Execute an SQL SELECT query on multiple tables.  Tables are joined
     * using an OUTER JOIN.  All attributes in all tables will be retrieved
     * using a hades::relations_accessor.
     *
     * \returns A styx::list of styx::objects.  Objects contain all keys from
     * all tuples in the join.
     */
    template<typename ...Tuples>
    styx::list equi_join(connection& conn)
    {
        return join_<detail::join_type::outer, true, Tuples...>(
                conn,
                filter_all()
                );
    }

    /*!
     * \brief Execute an SQL SELECT query on multiple tables.  Tables are joined
     * using an OUTER JOIN.  All attributes in all tables will be retrieved
     * using a hades::relations_accessor.
     *
     * \returns A styx::list of styx::objects.  Objects contain all keys from
     * all tuples in the join.
     */
    template<typename ...Tuples>
    styx::list equi_join(
            connection& conn,
            const basic_filter& filter_
            )
    {
        return join_<detail::join_type::outer, true, Tuples...>(
                conn,
                filter_
                );
    }

    /*!
     * \brief Execute an SQL SELECT query on multiple tables.  Tables are joined
     * using an OUTER JOIN.  All attributes in all tables will be retrieved
     * using a hades::relations_accessor.
     *
     * \returns A styx::list of styx::objects.  Objects contain all keys from
     * all tuples in the join.
     */
    template<typename ...Tuples>
    styx::list outer_join(connection& conn)
    {
        return join_<detail::join_type::outer, false, Tuples...>(
                conn,
                filter_all()
                );
    }

    /*!
     * \brief Execute an SQL SELECT query on multiple tables.  Tables are joined
     * using an OUTER JOIN.  All attributes in all tables will be retrieved
     * using a hades::relations_accessor.
     *
     * \returns A styx::list of styx::objects.  Objects contain all keys from
     * all tuples in the join.
     */
    template<typename ...Tuples>
    styx::list outer_join(
            connection& conn,
            const basic_filter& filter_
            )
    {
        return join_<detail::join_type::outer, false, Tuples...>(
                conn,
                filter_
                );
    }
}

#endif

