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

template int& styx::list::get<int>(styx::list::size_type);
template double& styx::list::get<double>(styx::list::size_type);
template std::string& styx::list::get<std::string>(styx::list::size_type);
template styx::list& styx::list::get<styx::list>(styx::list::size_type);
template styx::element& styx::list::get<styx::element>(styx::list::size_type);
