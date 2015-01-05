Tuple Types
===========

A template instantiation of hades::tuple is a model of a tuple in a relational
database (a row in SQLite).

The tuple is the result of a query constructed from one or more base tables
(relvars) or views.

The tuple class is parameterised on a list of attribute names provided as const
char \* (see section on [attributes](attributes.html)).

hades::tuple has templated get\_ functions.  These each accept the attribute
name as a template argument and provide safe access to that attribute
(attempting to access an attribute not in the tuple results in a compilation
error).

Example
-------

In a header file:

    #include "hades/tuple.hpp"

    namespace application
    {
        namespace db
        {
            namespace attr
            {
                namespace relation_name
                {
                    extern const char attribute_name[];
                }
            }
        }
        struct tuple_type :
            public hades::tuple<db::attr::relation_name::attribute_name>
        {
        };
    }

In the implementation file:

    const char application::db::attr::relation_name::attribute_name[] =
        "attribute_name";

Inside a function in the implementation file:

    tuple_type t;
    t.get_string<db::attr::relation_name::attribute_name>() = "example";

