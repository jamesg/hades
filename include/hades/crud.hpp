#ifndef HADES_CRUD_HPP
#define HADES_CRUD_HPP

#include "hades/get_collection.hpp"
#include "hades/detail/last_insert_rowid.hpp"
#include "hades/transaction.hpp"

namespace hades
{
    class connection;

    /*!
     * \brief Provide basic CRUD (create, read, update and delete) functions
     * for a database type.  Functions provide syntactic sugar for free
     * functions in namespace hades (get_collection, insert, save, update).
     *
     * \param Tuple Database structure for which to provide CRUD functions.
     * Must derive from hades::has_candidate_key and hades::tuple.
     *
     * \example
     * class new_db_struct :
     *     hades::tuple<...>,
     *     hades::has_candidate_key<...>,
     *     hades::crud<new_db_struct>
     */
    template<typename Tuple>
    class crud
    {
        public:
            //
            // Permanent storage functions.
            //

            static styx::list get_collection(hades::connection& conn)
            {
                return hades::get_collection<Tuple>(conn);
            }

            /*!
             * \brief Insert the tuple into the database.  The id of this tuple
             * will be set to the id of the database tuple.
             *
             * \note This function was written to prevent
             * "tuple.set_id(hades::insert(...))" in external code.
             */
            void insert(hades::connection& conn)
            {
                Tuple& t = static_cast<Tuple&>(*this);
                {
                    std::ostringstream query;
                    query << "INSERT INTO " << Tuple::relation_name << " ( ";
                    auto attributes = t.value_attributes();
                    styx::serialise(
                            attributes,
                            [](const char *s, std::ostream& os) {
                                os << s;
                            },
                            ", ",
                            query
                            );
                    query << " ) VALUES ( ";
                    styx::serialise(
                            attributes,
                            [](const char *, std::ostream& os) {
                                os << "?";
                            },
                            ", ",
                            query
                            );
                    query << ")";

#ifdef HADES_ENABLE_DEBUGGING
                    std::cerr << "hades insert query: " << query.str() << std::endl;
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
                        throw std::runtime_error(
                                mkstr() << "preparing SQLite statement \"" <<
                                    query.str() << "\" for insert (" <<
                                    sqlite3_errmsg(conn.handle()) << ")"
                                );
                    }

                    try
                    {
                        t.bind_set_values(stmt);
                    }
                    catch(const std::exception& e)
                    {
                        throw std::runtime_error(
                                mkstr() << "binding values to insert: " << e.what() <<
                                " query   \"" << query.str() << "\""
                                );
                    }

                    int step_ret = sqlite3_step(stmt);
                    if(step_ret != SQLITE_OK && step_ret != SQLITE_DONE)
                    {
                        std::ostringstream error_string;
                        error_string << "stepping SQLite insert " << query.str() <<
                            " " << sqlite3_errmsg(conn.handle());
                        throw std::runtime_error(error_string.str());
                    }

                    int finalise_ret = sqlite3_finalize(stmt);
                    if(finalise_ret != SQLITE_OK && finalise_ret != SQLITE_DONE)
                    {
                        std::ostringstream error_string;
                        error_string << "SQLite finalise " << query.str() << " " <<
                            sqlite3_errmsg(conn.handle());
                        throw std::runtime_error(error_string.str());
                    }
                }

                // Only attempt to retrieve the complete id if the type has id
                // attributes.
                if(Tuple::id_type::arity > 0)
                {
                    int last_id = detail::last_insert_rowid(conn);
                    std::ostringstream id_query;
                    id_query << "SELECT ";
                    Tuple::id_type::key_column_list(id_query);
                    id_query << " FROM " << Tuple::relation_name;
                    id_query << " WHERE rowid = ? ";
                    sqlite3_stmt *last_id_stmt = nullptr;
                    if(
                            sqlite3_prepare(
                                conn.handle(),
                                id_query.str().c_str(),
                                -1,
                                &last_id_stmt,
                                nullptr
                                ) != SQLITE_OK
                            )
                    {
                        throw std::runtime_error(
                                mkstr() << "preparing id query \"" << id_query.str() << "\""
                                );
                    }

                    try
                    {
                        hades::bind(1, last_id, last_id_stmt);
                    }
                    catch(const std::exception& e)
                    {
                        throw std::runtime_error(
                                mkstr() << "binding to id statement: " << e.what()
                                );
                    }

                    int step_ret = sqlite3_step(last_id_stmt);
                    if(step_ret != SQLITE_ROW)
                    {
                        throw std::runtime_error(
                            mkstr() << "stepping SQLite select last id \"" <<
                                id_query.str() << "\" " << sqlite3_errmsg(conn.handle())
                            );
                    }

                    typename Tuple::id_type id =
                        Tuple::id_type::from_stmt(last_id_stmt);
                    t.set_id(id);

                    int finalise_ret = sqlite3_finalize(last_id_stmt);
                    if(finalise_ret != SQLITE_OK && finalise_ret != SQLITE_DONE)
                    {
                        std::ostringstream error_string;
                        error_string << "SQLite finalise " << id_query.str() << " " <<
                            sqlite3_errmsg(conn.handle());
                        throw std::runtime_error(error_string.str());
                    }
                }
            }

            /*!
             * \brief Save the tuple to the database.  Updates the tuple if it
             * already has an id, inserts the tuple otherwise.  The id of this
             * tuple will be set to the id of the database tuple.
             *
             * \note This function was written to prevent
             * "tuple.set_id(hades::save(...))" in external code.
             */
            void save(connection& conn)
            {
                Tuple& t = static_cast<Tuple&>(*this);
                transaction tr(conn, "save_transaction");
                bool updated = false;
                try
                {
                    updated = update(conn);
                }
                catch(const std::exception& e)
                {
                    // An exception can be thrown if the update fails.
                }
                if(!updated)
                    insert(conn);
                tr.commit();
            }

            /*!
             * \brief Updates the tuple in the database.  The id of this tuple
             * will be set to the id of the database tuple.
             *
             * \note This function was written to prevent
             * "tuple.set_id(hades::save(...))" in external code.
             */
            bool update(connection& conn)
            {
                Tuple& t = static_cast<Tuple&>(*this);

                if(!t.id_set())
                    throw std::runtime_error("cannot update a tuple if id is incomplete");

                std::ostringstream oss;
                oss << "UPDATE " << Tuple::relation_name << " SET ";
                styx::serialise(
                        Tuple::attribute_list_type::to_vector(),
                        [](const char *s, std::ostream& os) {
                            os << s << " = ? ";
                        },
                        ", ",
                        oss
                        );
                oss << " WHERE ";

                Tuple::id_type::key_where_clause(oss);

#ifdef HADES_ENABLE_DEBUGGING
                    std::cerr << "hades update query: " << oss.str() << std::endl;
#endif

                sqlite3_stmt *stmt = nullptr;
                if(
                        sqlite3_prepare(
                            conn.handle(),
                            oss.str().c_str(),
                            -1,
                            &stmt,
                            nullptr
                            ) != SQLITE_OK
                        )
                {
                    throw std::runtime_error(
                            mkstr() << "hades::update: preparing UPDATE query \"" <<
                                oss.str() << "\": " << sqlite3_errmsg(conn.handle())
                            );
                }

                t.bind_values(stmt);
                t.template bind_key_values<Tuple::arity+1>(stmt);

                int step_ret = sqlite3_step(stmt);
                if(step_ret != SQLITE_OK && step_ret != SQLITE_DONE)
                {
                    throw std::runtime_error(
                            mkstr() << "stepping SQLite UPDATE query \"" <<
                                oss.str() << "\": " << sqlite3_errmsg(conn.handle())
                            );
                }

                int count = sqlite3_changes(conn.handle());

                int finalise_ret = sqlite3_finalize(stmt);
                if(finalise_ret != SQLITE_OK && finalise_ret != SQLITE_DONE)
                {
                    throw std::runtime_error(
                            mkstr() << "finalising SQLite UPDATE query \"" <<
                                oss.str() << "\": " << sqlite3_errmsg(conn.handle())
                            );
                }

                // The number of rows updated (0 or 1, because the target tuples were
                // selected by a candidate key).
                return (count > 0);
            }

            bool destroy(connection& conn)
            {
                Tuple& t = static_cast<Tuple&>(*this);
                std::ostringstream oss;
                oss << "DELETE FROM " << Tuple::relation_name << " WHERE ";
                Tuple::id_type::key_where_clause(oss);
#ifdef HADES_ENABLE_DEBUGGING
                std::cerr << "hades delete query: " << oss.str() << std::endl;
#endif

                sqlite3_stmt *stmt = nullptr;
                if(
                        sqlite3_prepare(
                            conn.handle(),
                            oss.str().c_str(),
                            oss.str().length(),
                            &stmt,
                            nullptr
                            ) != SQLITE_OK
                        )
                    throw std::runtime_error(
                            mkstr() << "preparing SQLite DELETE query \"" <<
                                oss.str() << "\" message: " <<
                                sqlite3_errmsg(conn.handle())
                            );
                auto id = t.id();
                Tuple::id_type::bind_key_values(id, stmt);
                if(sqlite3_step(stmt) != SQLITE_DONE)
                    throw std::runtime_error(
                            mkstr() << "stepping SQLite DELETE query \"" <<
                                oss.str() << "\" message: " <<
                                sqlite3_errmsg(conn.handle())
                            );
                int count = sqlite3_changes(conn.handle());
                int finalise_ret = sqlite3_finalize(stmt);
                if(finalise_ret != SQLITE_OK && finalise_ret != SQLITE_DONE)
                    throw std::runtime_error(
                            mkstr() << "finalising SQLite DELETE query \"" <<
                                oss.str() << "\" message: " <<
                                sqlite3_errmsg(conn.handle())
                            );
                return (count > 0);
            }
    };
}

#endif

