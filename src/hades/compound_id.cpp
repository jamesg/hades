#include "hades/compound_id.hpp"

template<>
void hades::compound_id<>::key_where_clause(std::ostream& os)
{
    os << "1";
}

