Create, Read, Update, Delete (CRUD)
===================================

The hades::crud template class provides standard create, read, update and
delete operations for tuple types.  Static functions can be used stand-alone
(with hades::crud templated on the tuple type), but in more typical use, a
tuple type will derive from hades::crud.

To speed up compilation, the implementation part of hades::crud is in a
separate .ipp file.  "hades/crud.ipp" or individual .ipp files in hades/crud/
must be included in any implementation file using a hades::crud function (.ipp
files should not be included from a header, this defeats the object of them).

Example
-------

    #include "hades/crud.hpp"

    struct tuple_type :
        hades::crud<tuple_type>,
        hades::relation<db::relvar::relation_name>,
        hades::tuple<db::attr::relation_name::attribute_name>

Member Functions
----------------

hades::crud will provide these member functions to classes:

    static styx::list get_collection(
        hades::connection& conn,
        hades::basic_filter& filter
        )

Get all tuples of this tuple type from the database.  The filter parameter is
optional, but if provided, tuples will be filtered accordingly.

    void from_id(hades::connection& conn, Tuple::id_type id)

Populate a tuple with data obtained by querying for an id.

    void insert(hades::connection& conn)

Insert the tuple into the database.  Id fields in the tuple will be set to the
id assigned by the database.

    bool update(hades::connection& conn)

Update an existing tuple in the database.  The tuple must exist and the id
attributes must be set.  Returns true if a tuple in the database matched, false
otherwise.

    bool save(hades::connection& conn)

Save a tuple by updating or inserting the tuple as appropriate.  Id attributes
in the tuple will be set if the tuple was inserted.  Returns true if a new
tuple was inserted, false if an existing tuple was updated.

    bool destroy(hades::connection& conn)

Delete the tuple from the database.  Does not modify the local tuple.  Returns
true if the tuple was deleted, false otherwise.

