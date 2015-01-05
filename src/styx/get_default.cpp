#include "styx/get_default.hpp"

#include "styx/list.hpp"
#include "styx/object.hpp"

template<> int styx::get_default<int>()
{
    return 0;
}

template<> double styx::get_default<double>()
{
    return 0.0;
}

template<> bool styx::get_default<bool>()
{
    return false;
}

template<> styx::element styx::get_default<styx::element>()
{
    return null_t();
}

