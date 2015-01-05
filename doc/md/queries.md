Queries
=======

Base Types
----------

Basic queries on base relations are covered in the
[basic tutorial](tutorial.html).

Joins
-----

Hades can construct SQL `JOIN` queries using either your own custom filter or
by joining on equality of fields which appear in both relations (like a natural
join).

Join Functions
--------------

### Cartesian Products

Both of these functions generate a Cartesian product.  The second version
allows the product to be selected with a `WHERE` clause.

        template<typename ...Tuples>
        styx::list join(connection&)

        template<typename ...Tuples>
        styx::list join(connection&, const basic_filter&)

### Equi (natural) Joins

        template<typename ...Tuples>
        styx::list equi_join(connection&)

        template<typename ...Tuples>
        styx::list equi_join(connection&, const basic_filter&)

        template<typename ...Tuples>
        styx::list equi_outer_join(connection&)

        template<typename ...Tuples>
        styx::list equi_outer_join(connection&, const basic_filter&)

### Outer Joins

All these functions execute `LEFT OUTER` joins.  As well as a `WHERE` filter,
`outer_join()` accepts an `ON` clause as a string.  This is an important
feature, as `ON` and `WHERE` clauses are handled differently in outer joins.

        template<typename ...Tuples>
        styx::list outer_join(connection&)

        template<typename ...Tuples>
        styx::list outer_join(connection&, const basic_filter&)

        template<typename ...Tuples>
        styx::list outer_join(connection&, const std::string& on)

        template<typename ...Tuples>
        styx::list outer_join(connection&, const std::string& on, const basic_filter&)

