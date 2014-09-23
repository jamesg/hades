#include "catch.hpp"

#include <sqlite3.h>

#include "hades/connection.hpp"
#include "hades/has_candidate_key.hpp"
#include "hades/relation.hpp"
#include "hades/tuple.hpp"
#include "hades/type_list.hpp"

namespace
{
    namespace attribute
    {
        namespace site
        {
            constexpr char site_id[] = "site_id";
            constexpr char name[] = "name";
        }
    }

    namespace relvar
    {
        constexpr char site[] = "site";
    }

    struct site :
        public hades::relation<relvar::site>,
        public hades::tuple<attribute::site::site_id, attribute::site::name>,
        public hades::type_list<int, std::string>,
        public hades::has_candidate_key<attribute::site::site_id>
    {
        site(styx::element& o) :
            styx::object_accessor(o)
        {
        }

        int& site_id()
        {
            return key<attribute::site::site_id>();
        }

        std::string& name()
        {
            return get_string<attribute::site::name>();
        }
    };
}

SCENARIO("type list") {
    hades::connection conn = hades::connection::in_memory_database();
}

