Hades Photograph Example
========================

This example uses Hades to define data structures for a photograph database
application.  Photographs have a title, caption and date taken.  They have zero
or one location and zero or many tags.

Note how attribute name strings are not repeated.  For instance,
`photograph_id` is used in the `photograph` and `photograph_in_album` relvars.
This allows Hades' `join` functions to generate joins in which these attributes
are equal.

photograph.hpp
--------------

        #ifndef HELIOS_DB_PHOTOGRAPH_HPP
        #define HELIOS_DB_PHOTOGRAPH_HPP

        #include <vector>

        #include "hades/crud.hpp"
        #include "hades/has_candidate_key.hpp"
        #include "hades/relation.hpp"
        #include "hades/tuple.hpp"

        namespace helios
        {
            namespace db
            {
                namespace attr
                {
                    namespace photograph
                    {
                        extern const char photograph_id[];
                        extern const char title[];
                        extern const char caption[];
                        extern const char taken[];
                    }
                    namespace photograph_tagged
                    {
                        extern const char tag[];
                    }
                    namespace photograph_location
                    {
                        extern const char location[];
                    }
                    namespace album
                    {
                        extern const char album_id[];
                        extern const char name[];
                    }
                    namespace location
                    {
                        extern const char location[];
                        extern const char photograph_count[];
                    }
                    namespace tag
                    {
                        extern const char tag[];
                        extern const char photograph_count[];
                    }
                }
                namespace relvar
                {
                    extern const char photograph[];
                    extern const char photograph_tagged[];
                    extern const char photograph_location[];
                    extern const char photograph_in_album[];
                    extern const char album[];
                }
            }
            struct photograph :
                public hades::has_candidate_key<db::attr::photograph::photograph_id>,
                public hades::tuple<
                    db::attr::photograph::photograph_id,
                    db::attr::photograph::title,
                    db::attr::photograph::caption,
                    db::attr::photograph::taken>,
                public hades::relation<db::relvar::photograph>,
                public hades::crud<photograph>
            {
                photograph()
                {
                }
                photograph(const styx::element& e) :
                    styx::object(e)
                {
                }
                std::string& tags() { return styx::object::get_string("tags"); }
            };
            struct photograph_in_album :
                public hades::has_candidate_key<
                    db::attr::photograph::photograph_id,
                    db::attr::album::album_id>,
                public hades::tuple<
                    db::attr::photograph::photograph_id,
                    db::attr::album::album_id>,
                public hades::relation<db::relvar::photograph_in_album>,
                public hades::crud<photograph_in_album>
            {
                photograph_in_album()
                {
                }
                photograph_in_album(styx::element& e) :
                    styx::object(e)
                {
                }
                photograph_in_album(id_type id) :
                    styx::object(id)
                {
                }
            };
            struct photograph_location :
                public hades::has_candidate_key<
                    db::attr::photograph::photograph_id>,
                public hades::tuple<
                    db::attr::photograph::photograph_id,
                    db::attr::photograph_location::location>,
                public hades::relation<db::relvar::photograph_location>,
                public hades::crud<photograph_location>
            {
                photograph_location()
                {
                }
                photograph_location(const styx::element& e) :
                    styx::object(e)
                {
                }
            };
            struct photograph_tagged :
                public hades::has_candidate_key<
                    db::attr::photograph::photograph_id,
                    db::attr::photograph_tagged::tag>,
                public hades::tuple<
                    db::attr::photograph::photograph_id,
                    db::attr::photograph_tagged::tag>,
                public hades::relation<db::relvar::photograph_tagged>,
                public hades::crud<photograph_tagged>
            {
                photograph_tagged()
                {
                }
                photograph_tagged(styx::element& e) :
                    styx::object(e)
                {
                }
            };
            struct album :
                public hades::has_candidate_key<db::attr::album::album_id>,
                public hades::tuple<
                    db::attr::album::album_id,
                    db::attr::album::name>,
                public hades::relation<db::relvar::album>,
                public hades::crud<album>
            {
                album()
                {
                }
                album(const styx::element& e) :
                    styx::object(e)
                {
                }
            };
            /*!
             * \brief View onto photograph_location.
             */
            struct location :
                public hades::tuple<
                    db::attr::location::location,
                    db::attr::location::photograph_count>,
                public hades::relation<db::relvar::photograph_location>
            {
            };
            /*!
             * \brief Location details.
             */
            struct basic_location :
                public hades::tuple<db::attr::location::location>,
                public hades::relation<db::relvar::photograph_location>
            {
            };
            /*!
             * \brief View onto photograph_tagged.
             */
            struct tag :
                public hades::tuple<
                    db::attr::tag::tag,
                    db::attr::tag::photograph_count>,
                public hades::relation<db::relvar::photograph_tagged>
            {
            };
            /*!
             * \brief Tag details.
             */
            struct basic_tag :
                public hades::tuple<db::attr::tag::tag>,
                public hades::relation<db::relvar::photograph_tagged>
            {
            };

            namespace db
            {
                namespace photograph
                {
                    void create(hades::connection& photograph_db);
                }

                styx::list get_photographs_by_album(
                        hades::connection&,
                        const int album_id
                        );
                std::vector<std::string> photograph_tags(
                        hades::connection&,
                        helios::photograph::id_type
                        );
                void set_photograph_tags(
                        hades::connection&,
                        helios::photograph::id_type,
                        std::vector<std::string> tags
                        );
                void set_photograph_tags(
                        hades::connection&,
                        helios::photograph::id_type,
                        std::string tags
                        );
            }
        }

        #endif

photograph.cpp
--------------

The implementation file provides definitions of all strings used as relvar and
attribute names.  It also provides a definition of the `create` function (Hades
does not create database tables).

        #include "photograph.hpp"

        #include <boost/tokenizer.hpp>

        #include "hades/crud.ipp"
        #include "hades/devoid.hpp"
        #include "hades/filter.hpp"
        #include "hades/join.hpp"

        const char helios::db::attr::photograph::photograph_id[] = "photograph_id";
        const char helios::db::attr::photograph::title[] = "title";
        const char helios::db::attr::photograph::caption[] = "caption";
        const char helios::db::attr::photograph::taken[] = "taken";
        const char helios::db::attr::photograph_tagged::tag[] = "tag";
        const char helios::db::attr::photograph_location::location[] = "location";
        const char helios::db::attr::album::album_id[] = "album_id";
        const char helios::db::attr::album::name[] = "name";
        const char helios::db::attr::tag::tag[] = "tag";
        const char helios::db::attr::tag::photograph_count[] = "photograph_count";
        const char helios::db::attr::location::location[] = "location";
        const char helios::db::attr::location::photograph_count[] = "photograph_count";
        const char helios::db::relvar::photograph[] = "photograph";
        const char helios::db::relvar::photograph_tagged[] = "photograph_tagged";
        const char helios::db::relvar::photograph_location[] = "photograph_location";
        const char helios::db::relvar::photograph_in_album[] = "photograph_in_album";
        const char helios::db::relvar::album[] = "album";

        void helios::db::photograph::create(hades::connection& conn)
        {
            hades::devoid(
                    "CREATE TABLE IF NOT EXISTS photograph ( "
                    " photograph_id INTEGER PRIMARY KEY AUTOINCREMENT, "
                    " title VARCHAR NOT NULL, "
                    " caption VARCHAR NULL, "
                    " taken VARCHAR NULL "
                    " ) ",
                    conn
                    );
            hades::devoid(
                    "CREATE TABLE IF NOT EXISTS photograph_location ( "
                    " photograph_id INTEGER PRIMARY KEY, "
                    " location VARCHAR, "
                    " FOREIGN KEY(photograph_id) REFERENCES photograph(photograph_id) "
                    "  ON DELETE CASCADE DEFERRABLE INITIALLY DEFERRED "
                    " ) ",
                    conn
                    );
            hades::devoid(
                    "CREATE TABLE IF NOT EXISTS jpeg_data ( "
                    " photograph_id INTEGER PRIMARY KEY, "
                    " data BLOB NOT NULL, "
                    " FOREIGN KEY(photograph_id) REFERENCES photograph(photograph_id) "
                    "  ON DELETE CASCADE DEFERRABLE INITIALLY DEFERRED "
                    " ) ",
                    conn
                    );
            hades::devoid(
                    "CREATE TABLE IF NOT EXISTS album ( "
                    " album_id INTEGER PRIMARY KEY AUTOINCREMENT, "
                    " name VARCHAR NOT NULL, "
                    " caption VARCHAR NULL, "
                    " UNIQUE(name) "
                    " ) ",
                    conn
                    );
            hades::devoid(
                    "CREATE TABLE IF NOT EXISTS photograph_tagged ( "
                    " photograph_id INTEGER NOT NULL, "
                    " tag VARCHAR NOT NULL, "
                    " FOREIGN KEY(photograph_id) REFERENCES photograph(photograph_id) "
                    "  ON DELETE CASCADE DEFERRABLE INITIALLY DEFERRED, "
                    " UNIQUE(photograph_id, tag) "
                    " ) ",
                    conn
                    );
            hades::devoid(
                    "CREATE TABLE IF NOT EXISTS photograph_in_album ( "
                    " photograph_id INTEGER NOT NULL, "
                    " album_id INTEGER NOT NULL, "
                    " FOREIGN KEY(photograph_id) REFERENCES photograph(photograph_id) "
                    "  ON DELETE CASCADE DEFERRABLE INITIALLY DEFERRED, "
                    " FOREIGN KEY(album_id) REFERENCES album(album_id) "
                    "  ON DELETE CASCADE DEFERRABLE INITIALLY DEFERRED, "
                    " UNIQUE(photograph_id, album_id) "
                    " ) ",
                    conn
                    );
        }

        styx::list helios::db::get_photographs_by_album(
                hades::connection& conn,
                const int album_id
                )
        {
            boost::fusion::vector<int> params(album_id);
            return hades::join<helios::photograph, helios::photograph_in_album>(
                    conn,
                    hades::where("album_id = ?", hades::row<int>(album_id))
                    );
        }

        std::vector<std::string> helios::db::photograph_tags(
                hades::connection& conn,
                helios::photograph::id_type id
                )
        {
            auto where = hades::where(
                "photograph_id = ?",
                hades::row<int>(
                    id.get_int<db::attr::photograph::photograph_id>()
                    )
                );
            styx::list tags = helios::photograph_tagged::get_collection(conn, where);
            std::vector<std::string> out;
            for(styx::element e : tags)
                out.push_back(
                        helios::photograph_tagged(e)
                            .get_string<db::attr::photograph_tagged::tag>()
                        );
            return out;
        }

        void helios::db::set_photograph_tags(
                hades::connection& conn,
                helios::photograph::id_type id,
                std::vector<std::string> tags
                )
        {
            hades::transaction tr(conn, "helios_db_set_photograph_tags");
            hades::devoid(
                    "DELETE FROM photograph_tagged WHERE photograph_id = ?",
                    hades::row<int>(id.get_int<db::attr::photograph::photograph_id>()),
                    conn
                    );
            for(const std::string& tag : tags)
            {
                helios::photograph_tagged t;
                t.get_int<db::attr::photograph::photograph_id>() =
                    id.get_int<db::attr::photograph::photograph_id>();
                t.get_string<db::attr::photograph_tagged::tag>() = tag;
                t.insert(conn);
            }
            tr.commit();
        }

        void helios::db::set_photograph_tags(
                hades::connection& conn,
                helios::photograph::id_type id,
                std::string tags
                )
        {
            std::vector<std::string> tags_vector;
            typedef boost::tokenizer<boost::escaped_list_separator<char>> tokenizer;
            tokenizer t(tags, boost::escaped_list_separator<char>("\\", " ", "\""));
            for(auto it = t.begin(); it != t.end(); ++it)
                tags_vector.push_back(*it);
            set_photograph_tags(conn, id, tags_vector);
        }

