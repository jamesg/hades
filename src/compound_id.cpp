#include "hades/compound_id.hpp"

template<>
void hades::compound_id<>::key_where_clause(std::ostream& os)
{
    os << "1";
}

template<>
void hades::compound_id<>::copy_attributes_from_id_map(
        detail::compound_id_map<>,
        styx::object_accessor&
        )
{
}

