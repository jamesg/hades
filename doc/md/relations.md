Relations
=========

The relation type is an indicator to Hades functions that a tuple type is
queried from a relation.  Tuple types should extend from hades::relation to
enable query functions.

Relation named should be defined as const char [] variables in an appropriate
namespace, as per attribute names.

Example
-------

    #include "hades/relation.hpp"

    struct tuple_type :
        hades::relation<db::relvar::relation_name>,
        hades::tuple<db::attr::relation_name::attribute_name>

