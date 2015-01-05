#include "styx/styx.hpp"

#include "styx/element.hpp"

bool styx::is_null(const styx::element& e)
{
    return boost::get<styx::null_t>(&e) != nullptr;
}

