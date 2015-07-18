#include "styx/list.hpp"

#include "styx/cast.hpp"
#include "styx/object.hpp"

styx::element& styx::list::operator[](int i)
{
    return elements[i];
}

styx::list::size_type styx::list::size() const
{
    return elements.size();
}

void styx::list::append( const element& obj )
{
    elements.push_back(obj);
}

void styx::list::append_list(const element& obj)
{
    const list& l = boost::get<list>(obj);
    for(auto o : l.elements)
        append(o);
}

void styx::list::prepend(const element& o)
{
    elements.push_front(o);
}

void styx::list::prepend_list(const element& o)
{
    const list& l = boost::get<list>(o);
    for(auto i = l.elements.crbegin(); i != l.elements.crend(); ++i)
        prepend(*i);
}

template<typename T>
T& styx::list::get(size_type k)
{
    elements[k] = styx::cast<T>(elements[k]);
    return const_cast<T&>(boost::get<T>(elements.at(k)));
}

const char *styx::empty_list_exception::what() const noexcept
{
    return "empty list";
}

const char *styx::list_out_of_bounds_exception::what() const noexcept
{
    return "list index out of bounds";
}

styx::element styx::first(styx::list&& l)
{
    if(l.size() > 0)
        return l.at(0);
    throw empty_list_exception();
}

const styx::element& styx::first(const styx::list& l)
{
    if(l.size() > 0)
        return l.at(0);
    throw empty_list_exception();
}

styx::element& styx::first(styx::list& l)
{
    if(l.size() > 0)
        return l[0];
    throw empty_list_exception();
}

template styx::int_type& styx::list::get<styx::int_type>(styx::list::size_type);
template double& styx::list::get<double>(styx::list::size_type);
template std::string& styx::list::get<std::string>(styx::list::size_type);
template styx::list& styx::list::get<styx::list>(styx::list::size_type);
template styx::object& styx::list::get<styx::object>(styx::list::size_type);
template styx::element& styx::list::get<styx::element>(styx::list::size_type);
