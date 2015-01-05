Tutorial
========

1. Opening a database connection.  This is straightforward enough, just
   construct a hades::connection with the filename of the SQLite database file.
   The file will be created if it does not already exist.

        // Provides hades::connection.
        #include "hades/connection.hpp"
        hades::connection conn("database.db");

   SQLite also provides in-memory databases.  To open an in-memory database,
   use hades::connection::in\_memory\_database().

        hades::connection conn = hades::connection::in_memory_database();

   Connection objects can be moved but not copied.  If a connection object does
   not represent an active connection, queries on it will throw an exception.

2. Running devoid SQL statements (statements which do not return a result).
   Use hades::devoid to execute the query.

        // Provides hades::devoid.
        #include "hades/devoid.hpp"
        hades::devoid("CREATE TABLE IF NOT EXISTS ...", conn);

   Hades will not generate CREATE TABLE statements for you, so this is the
   preferred way to initialise the database.

   If you need to provide arguments to the statement, this can be done with
   SQLite's poisitional parameters and a hades::row.

        // Provides hades::row.
        #include "hades/row.hpp"
        hades::devoid(
            "INSERT INTO test VALUES(?)",
            hades::row<std::string>("test"),
            conn
            );

   Of course, the purpose of an ORM library is to avoid having to write
   statements like this, so read on!

3. Defining a type.  Like most object relational mappers, a type (class)
   corresponds to a relvar (SQL base table), while instances of the type
   (objects) correspond to tuples (SQL rows).

        // Provides hades::relation.
        #include "hades/relation.hpp"
        // Provides hades::tuple.
        #include "hades/tuple.hpp"

        namespace application
        {
            namespace db
            {
                // This namespacing convention is optional, but helps separate
                // attribute and relvar names.
                namespace attr
                {
                    namespace relation_name
                    {
                        // Strings must be declared as 'extern const char []'
                        // in the header and defined in the implementation file.
                        extern const char id[];
                        extern const char data[];
                    }
                }
                namespace relvar
                {
                    extern const char relation_name[];
                }
            }
            // This is the type which we'll use to interact with the database.
            // It provides storage for one tuple (SQL row).
            struct tuple_type :
                // Inheriting from hades::relation gives Hades the name of the
                // relation to use for this type.
                hades::relation<db::relvar::relation_name>,
                // Inheriting from hades::has_candidate_key tells Hades which
                // attributes form a compound id.
                hades::has_candidate_key<db::attr::relation_name::id>,
                // Inheriting from hades::tuple specifies all attributes in the
                // tuple.
                hades::tuple<
                    db::attr::relation_name::id,
                    db::attr::relation_name::data>,
                // Inheriting from hades::crud attaches database functions such
                // as insert and update to the type.
                hades::crud<tuple_type>
            {
            public:
                tuple_type()
                {
                }
                // Enable construction from a styx::element.  This type of
                // constructor is used, for example, when the tuple has been
                // received as a JSON object from a web client.  styx::object
                // is a virtual base of hades::tuple.  This constructor may be
                // omitted.
                tuple_type(const styx::element& e) :
                    styx::object(e)
                {
                }
            };
        }

   Note: has\_candidate\_key accepts a list of arguments.

   Note: Attributes in the candidate key must be repeated in the hades::tuple
   argument list.

4. 'Saving' an object will attempt to update an existing object with the same
   id but will insert a new object is none matched.  This is simpler than
   choosing between insert() and update().

        #include "hades/crud.ipp"
        tuple.save(conn);

   If you are certain that you want an INSERT or UPDATE, use either the
   insert() or update() functions in hades::crud.

        tuple.insert(conn);

        tuple.update(conn);

5. Retrieving a tuple by its id.

        #include "hades/get_by_id.hpp"
        tuple_type tuple2 = hades::get_by_id<tuple_type>(conn, tuple.id());

6. Retrieving a collection of tuples.

        #include "hades/crud.ipp"
        styx::list collection = tuple_type::get_collection(conn);

7. Selecting a collection of tuples.

        #include "hades/crud.ipp"
        #include "hades/filter.hpp"
        styx::list collection = tuple_type::get_collection(
            conn,
            hades::where("member = 'Data'")
            );

8. Some more filters.

        #include "hades/crud.ipp"
        #include "hades/filter.hpp"
        #include "hades/row.hpp"
        std::string data("Data");
        // Inject the string 'Data' using a SQLite placeholder.  Hades will
        // call SQLite's native sqlite3_bind_text function.
        styx::list collection = tuple_type::get_collection(
            conn,
            hades::where("member = ?", hades::row<std::string>(data))
            );
        // Select five results where member = 'Data', ordered by id, omitting
        // the first result.
        styx::list collection = tuple_type::get_collection(
            conn,
            hades::filter(
                hades::where("member = 'Data'"),
                hades::order_by("id ASC", 5, 1)
                )
            );

