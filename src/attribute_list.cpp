#include "hades/attribute_list.hpp"

template<>
void hades::attribute_list<>::column_list(std::ostream&)
{
}

template<>
void hades::attribute_list<>::copy_attributes(
    styx::object_accessor&,
    styx::object_accessor&
    )
{
}

template<>
std::vector<const char*> hades::attribute_list<>::to_vector()
{
    return std::vector<const char*>();
}

template<>
bool hades::attribute_list<>::all_attributes_set(styx::object_accessor& obj)
{
    return true;
}

