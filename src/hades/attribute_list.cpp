#include "styx/list.hpp"
#include "styx/object.hpp"
#include "hades/attribute_list.hpp"

template<>
void hades::attribute_list<>::column_list(std::ostream&)
{
}

template<>
void hades::attribute_list<>::copy_attributes(
    styx::object&,
    styx::object&
    )
{
}

template<>
std::vector<const char*> hades::attribute_list<>::to_vector()
{
    return std::vector<const char*>();
}

template<>
bool hades::attribute_list<>::all_attributes_set(styx::object& obj)
{
    return true;
}

