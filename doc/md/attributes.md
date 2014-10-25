Attributes
==========

Attribute (column) names must be stored in one location by the application so
that they may be used as template arguments.  Strings should be stored as
'const char []', for example 'const char attr[] = "attr";'.

It is advisable to use a namespace convention for declaring attribute names.  For example, in a header file:

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
    }

And in the implementation file:

    const char application::db::attr::relation_name::attribute_name[] =
        "attribute_name";

Later, relation names will be stored in the same way.

The attribute must be defined in an implementation (.cpp) file to ensure that
it is only defined once.  This allows attributes to be compared by memory
address.  Errors such as accessing an attribute not present in a tuple, or
joining relations with different key types, can be detected by the compiler.

If an attribute is an id attribute, choose one place to define it.  This will
allow the attribute to be used in automatic joins.

