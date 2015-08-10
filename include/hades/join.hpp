#ifndef HADES_JOIN_HPP
#define HADES_JOIN_HPP

#ifdef HADES_ENABLE_DEBUGGING
#include <iostream>
#endif
#include <type_traits>

#include <sqlite3.h>

#include "hades/attribute_list.hpp"
#include "hades/conditional_value.hpp"
#include "hades/connection.hpp"
#include "hades/exception.hpp"
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
        // Helper type to turn a list of tuple types into a list of std::string.
        template<typename Tuple>
        struct on_clause_string
        {
            typedef std::string type;
        };

        //
        // on_relation_list: write a list of relation names and 'ON' clauses
        // seperated by a join type to a stream.
        //

        template<const char *Join, typename Relation>
        void on_relation_list_(
            typename detail::on_clause_string<Relation>::type clause,
            std::ostream& os
            )
        {
            os << Relation::relation_name;
            if(clause != "") {
                os << " ON " << clause;
            }
        }

        template<const char *Join, typename Relation1, typename Relation2, typename ...Relations>
        void on_relation_list_(
            typename detail::on_clause_string<Relation1>::type clause1,
            typename detail::on_clause_string<Relation2>::type clause2,
            typename detail::on_clause_string<Relations>::type... clauses,
            std::ostream& os
            )
        {
            os << Relation1::relation_name;
            if(clause1 != "") {
                os << " ON " << clause1;
            }
            os << Join;
            on_relation_list_<Join, Relation2, Relations...>(clause2, clauses..., os);
        }

        template<const char *Join, typename Relation1, typename Relation2, typename ...Relations>
        void on_relation_list(
            typename detail::on_clause_string<Relation2>::type clause,
            typename detail::on_clause_string<Relations>::type... clauses,
            std::ostream& os
            )
        {
            os << Relation1::relation_name;
            os << Join;
            on_relation_list_<Join, Relation2, Relations...>(clause, clauses..., os);
        }

        template<const char *Join, typename Relation1>
        void on_relation_list(std::ostream& os)
        {
            os << Relation1::relation_name;
        }

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
        // equijoin_relation_list: write a list of relations and join conditions to a stream.
        //

        template<const char *Join, typename RelationBase>
        void equijoin_relation_list_(std::ostream&)
        {
            // Do nothing; the name of RelationBase will already have been
            // appended.
        }

        template<const char *Join, typename RelationBase, typename RelationLast>
        void equijoin_relation_list_(std::ostream& os)
        {
            os << Join << " " << RelationLast::relation_name << " ON ";
            RelationBase::key_attribute_list::template equijoin_on_clause<
                (const char*)RelationBase::relation_name,
                (const char*)RelationLast::relation_name>(os);
        }

        template<
                const char *Join,
                typename RelationBase,
                typename Relation1,
                typename Relation2,
                typename ...Relations>
        void equijoin_relation_list_(std::ostream& os)
        {
            equijoin_relation_list_<Join, RelationBase, Relation1>(os);
            os << " ";
            equijoin_relation_list_<Join, RelationBase, Relation2, Relations...>(os);
        }

        template<const char *Join, typename RelationBase, typename ...Relations>
        void equijoin_relation_list(std::ostream& os)
        {
            os << RelationBase::relation_name;
            equijoin_relation_list_<Join, RelationBase, Relations...>(os);
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
            on_clause<Relation1, Relation3, Relations...>(os);
        }

        //
        // all_column_list: write the attribute list part of the SELECT
        // statement.  Behaviour depends on the type of tuple.  For standard
        // tuples, list all attributes.  For flags, extend the result with a
        // boolean attribute indicating whether the flag is present or not.
        //

        //
        // First version: write a comma-separated list of extended columns of
        // the form '(t.id IS NOT NULL) as t_exists'.  Used to recover boolean
        // flags from the result of a join.
        //
        template<typename Flag>
        typename std::enable_if<std::is_base_of<detail::basic_flag, Flag>::value>::type
        all_column_list(std::ostream& os)
        {
            os << "(" << Flag::relation_name << "." <<
                Flag::first_key_attr << " IS NOT NULL) AS " <<
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
        // record them in an object.  Values are retrieved starting at
        // an index supplied as the first template parameter.
        //
        // Again, there are two versions.  The flag version gets the boolean
        // attribute indicating presence of the tuple.  The tuple version gets
        // all attributes from the tuple (using
        // attribute_list::retrieve_values).
        //

        template<int Start, bool SkipKeys, typename Flag>
        typename std::enable_if<std::is_base_of<detail::basic_flag, Flag>::value>::type
        retrieve_tuple_values(
                sqlite3_stmt *stmt,
                styx::object& accessor
                )
        {
            int out = 0;
            get_column(stmt, Start, out);
#ifdef HADES_ENABLE_DEBUGGING
            std::cerr << "hades join retrieve flag value: index " << Start <<
                " relation " << Flag::relation_name << " value " << out <<
                std::endl;
#endif
            accessor.get_bool(Flag::relation_name) = (out > 0);
        }

        template<int Start, bool SkipKeys, typename Tuple>
        typename std::enable_if<!std::is_base_of<detail::basic_flag, Tuple>::value>::type
        retrieve_tuple_values(
                sqlite3_stmt *stmt,
                styx::object& accessor
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
            //typedef typename std::conditional<
                //std::is_base_of<detail::basic_flag, Tuple>::value,
                //arity_one,
                //arity_copy<Tuple>
                //>::type arity_type;
            //static constexpr const size_t value = arity_type::value;

            static constexpr const size_t value = hades::conditional_value<
                    size_t,
                    std::is_base_of<detail::basic_flag, Tuple>::value,
                    1,
                    Tuple::arity
                    >::type::value;
        };

        //
        // join_values: Traits type to retrieve values from the result of a join
        // (a set of relations).
        //

        template<bool SkipKeys, typename ...Relations>
        struct join_values
        {
            static void retrieve_join_values(
                    sqlite3_stmt *stmt,
                    styx::object& accessor
                    )
            {
                retrieve_join_values_<0, Relations...>(stmt, accessor);
            }
        private:
            template<int Start, typename Rel>
            static void retrieve_join_values_(
                    sqlite3_stmt *stmt,
                    styx::object& accessor
                    )
            {
                // Skip retrieving keys for all but the first tuple.
                retrieve_tuple_values<Start, SkipKeys && (Start != 0), Rel>(stmt, accessor);
            }

            template<int Start, typename Rel1, typename Rel2, typename ...Rels>
            static void retrieve_join_values_(
                    sqlite3_stmt *stmt,
                    styx::object& accessor
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

    namespace detail
    {
        template<bool SkipKeys, typename ...Tuples>
        styx::list fetch_join_result(sqlite3_stmt *stmt)
        {
            styx::list list;

            while(sqlite3_step(stmt) == SQLITE_ROW)
            {
                try
                {
                    styx::object object;
                    detail::join_values<SkipKeys, Tuples...>::retrieve_join_values(
                            stmt,
                            object
                            );
                    list.append(styx::element(object));
                }
                catch(const std::exception&)
                {
                    sqlite3_finalize(stmt);
                    throw;
                }
            }

            return list;
        }
    }

    /*!
     * \brief Execute an SQL SELECT query on multiple tables.  All attributes
     * of all relations used in the JOIN are retrieved.
     *
     * \internal
     *
     * \param Join Type of join (see namespace hades::detail::join_type).
     * \param Tuples Types inheriting from hades::tuple and hades::relation.
     * Determines the attributes and relation that will be queried.
     * \param filter SQL filter clause to use for the query.
     */
    template<const char *Join, bool SkipKeys, bool EquiJoin, typename Tuple, typename ...Tuples>
    styx::list join_on_(
            connection& conn,
            typename detail::on_clause_string<Tuples>::type... clauses,
            const basic_filter& filter_
            )
    {
        std::ostringstream query;
        query << "SELECT ";
        detail::all_column_list<Tuple, Tuples...>(query);
        query << " FROM ";
        if(EquiJoin)
            detail::equijoin_relation_list<Join, Tuple, Tuples...>(query);
        else
            detail::on_relation_list<Join, Tuple, Tuples...>(clauses..., query);
        //if(sizeof...(Tuples) > 1)
            //detail::equijoin_on_clause<EquiJoin, Tuple, Tuples...>(query);
        query << " " << filter_.clause();

#ifdef HADES_ENABLE_DEBUGGING
        std::cerr << "hades join query: \"" << query.str() << "\"" << std::endl;
#endif

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
            throw hades::exception(
                    mkstr() << "preparing SQLite statement \"" <<
                        query.str() << "\" (" <<
                        sqlite3_errmsg(conn.handle()) << ")"
                    );
        }
        filter_.bind(stmt);
        styx::list out = detail::fetch_join_result<SkipKeys, Tuple, Tuples...>(stmt);
        if(sqlite3_finalize(stmt) != SQLITE_OK)
            throw hades::exception("finalizing SQLite statement");
        return out;
    }

    /*!
     * \brief Execute an SQL SELECT query on multiple tables.  All attributes
     * of all relations used in the JOIN are retrieved.
     *
     * \internal
     *
     * \param Join Type of join (see namespace hades::detail::join_type).
     * \param Tuples Types inheriting from hades::tuple and hades::relation.
     * Determines the attributes and relation that will be queried.
     * \param filter SQL filter clause to use for the query.  Question mark
     * placeholders are filled with values in 'values'.
     */
    template<const char *Join, bool SkipKeys, bool EquiJoin, typename ...Tuples>
    styx::list join_(
            connection& conn,
            const basic_filter& filter_
            )
    {
        std::ostringstream query;
        query << "SELECT ";
        detail::all_column_list<Tuples...>(query);
        query << " FROM ";
        if(EquiJoin)
            detail::equijoin_relation_list<Join, Tuples...>(query);
        else
            detail::relation_list<Join, Tuples...>(query);
        //if(sizeof...(Tuples) > 1)
            //detail::equijoin_on_clause<EquiJoin, Tuples...>(query);
        query << " " << filter_.clause();

#ifdef HADES_ENABLE_DEBUGGING
        std::cerr << "hades join query: \"" << query.str() << "\"" << std::endl;
#endif

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
            throw hades::exception(
                    mkstr() << "preparing SQLite statement \"" <<
                        query.str() << "\" (" <<
                        sqlite3_errmsg(conn.handle()) << ")"
                    );
        }
        filter_.bind(stmt);
        styx::list out = detail::fetch_join_result<SkipKeys, Tuples...>(stmt);
        if(sqlite3_finalize(stmt) != SQLITE_OK)
            throw hades::exception("finalizing SQLite statement");
        return out;
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
        return join_<detail::join_type::inner, false, false, Tuple...>(
                conn,
                filter_
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
    template<typename ...Tuples>
    styx::list equi_join(connection& conn)
    {
        return join_<detail::join_type::inner, true, true, Tuples...>(
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
    template<typename ...Tuples>
    styx::list equi_join(
            connection& conn,
            const basic_filter& filter_
            )
    {
        return join_<detail::join_type::inner, true, true, Tuples...>(
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
    styx::list equi_outer_join(connection& conn)
    {
        return join_<detail::join_type::outer, true, true, Tuples...>(
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
    styx::list equi_outer_join(
            connection& conn,
            const basic_filter& filter_
            )
    {
        return join_<detail::join_type::outer, true, true, Tuples...>(
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
    template<typename Tuple, typename ...Tuples>
    styx::list outer_join(
            connection& conn,
            typename detail::on_clause_string<Tuples>::type... clauses
            )
    {
        return join_on_<detail::join_type::outer, false, false, Tuple, Tuples...>(
                conn,
                clauses...,
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
    template<typename Tuple, typename ...Tuples>
    styx::list outer_join(
            connection& conn,
            typename detail::on_clause_string<Tuples>::type... clauses,
            const basic_filter& filter_
            )
    {
        return join_on_<detail::join_type::outer, false, false, Tuple, Tuples...>(
                conn,
                clauses...,
                filter_
                );
    }

    // /*!
    //  * \brief Execute an SQL SELECT query on multiple tables.  Tables are joined
    //  * using an OUTER JOIN.  All attributes in all tables will be retrieved
    //  * using a hades::relations_accessor.
    //  *
    //  * \returns A styx::list of styx::objects.  Objects contain all keys from
    //  * all tuples in the join.
    //  */
    // template<typename ...Tuples>
    // styx::list outer_join(connection& conn, const std::string& on)
    // {
    //     return join_on_<detail::join_type::outer, false, Tuples...>(
    //             conn,
    //             on,
    //             filter_all()
    //             );
    // }
    //
    // /*!
    //  * \brief Execute an SQL SELECT query on multiple tables.  Tables are joined
    //  * using an OUTER JOIN.  All attributes in all tables will be retrieved
    //  * using a hades::relations_accessor.
    //  *
    //  * \returns A styx::list of styx::objects.  Objects contain all keys from
    //  * all tuples in the join.
    //  */
    // template<typename ...Tuples>
    // styx::list outer_join(
    //         connection& conn,
    //         const std::string& on,
    //         const basic_filter& filter_
    //         )
    // {
    //     return join_on_<detail::join_type::outer, false, Tuples...>(
    //             conn,
    //             on,
    //             filter_
    //             );
    // }
}

#endif
