#ifndef HADES_TYPE_LIST_HPP
#define HADES_TYPE_LIST_HPP

#include "styx/get_default.hpp"

#include "hades/bind_values.hpp"

namespace hades
{
    class connection;

    template<typename Type, typename ...Types>
    struct type_list
    {
        typedef type_list<Type, Types...> type_list_type;

        template<int Start, const char *Attr>
        static void bind_sqlite(sqlite3_stmt* stmt)
        {
            Type value = styx::get_default<Type>();
            hades::bind(Start, value, stmt);
        }
    };

    template<typename Type1, typename Type2, typename ...Types>
    struct type_list<Type1, Type2, Types...>
    {
        typedef type_list<Type1, Type2, Types...> type_list_type;

        template<int Start, const char *Attr1, const char *Attr2, const char *...Attrs>
        static void bind_sqlite(sqlite3_stmt* stmt)
        {
            type_list<Type1>::template bind_sqlite<Start+1, Attr1>(stmt);
            type_list<Type2, Types...>::template bind_sqlite<Start+1, Attr2, Attrs...>(stmt);
        }
    };
}

#endif

