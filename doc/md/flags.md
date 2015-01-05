Flags
=====

Very often, you'll want to attach one or more boolean properties to a tuple,
without making them a part of the tuple.  For instance, a 'user' type might
have attributes 'enabled' and 'is\_superuser'.

A boolean flag can be stored in a table with attributes matching the candidate key attributes of the associated type.  The presence of a tuple in this auxiliary indicates that the flag is true.  The absence of a tuple in the auxiliary table indicates that the flag is false.

Example
-------

These tables represent a user account and the predicate "The user account
identified by user\_id is enabled".

        CREATE TABLE user (
            user_id INTEGER AUTOINCREMENT PRIMARY KEY,
            username VARCHAR
        );
        CREATE TABLE user_enabled (
            user_id INTEGER REFERENCES user(user_id) PRIMARY KEY
        );

The 'user' tuple type should inherit from hades::has\_flags, which is templated
on the list of flags.  Here is the complete user type.

        #include "hades/has_flags.hpp"
        #include "hades/relation.hpp"
        #include "hades/tuple.hpp"

        namespace application
        {
            namespace db
            {
                namespace attr
                {
                    namespace user
                    {
                        extern const char user_id[];
                        extern const char username[];
                    }
                }
                // As in the basic tutorial, there is a strict convention for
                // naming these string constants.
                namespace flag
                {
                    namespace user
                    {
                        // This string is the name of the SQL table which holds
                        // the flag.
                        extern const char enabled[];
                    }
                }
                namespace relvar
                {
                    extern const char user[];
                }
            }
            struct user :
                hades::relation<db::relvar::user>,
                hades::has_candidate_key<db::attr::user::user_id>,
                hades::tuple<
                    db::attr::user::user_id,
                    db::attr::user::username>,
                hades::crud<user>,
                hades::has_flags<db::flag::user::enabled>
            {
            public:
                user()
                {
                }
                user(const styx::element& e) :
                    styx::object(e)
                {
                }
            };
        }

Is that all?  Not quite - we just need a way to access the flags.
hades::has\_flags will have added a get\_flag member function to the tuple
type.  This function works just like the other get\_ functions in that it is
templated on the flag's string.

        user u;
        u.get_flag<db::flag::user::enabled>() = true;

Functions attached to the tuple type by hades::crud (including insert, update,
save and from\_id) and hades::get\_by\_id all retrieve and save the additional
flags automatically.  hades::crud::get\_collection does not obtain the
additional flags for performance reasons.

