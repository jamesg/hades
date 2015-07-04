#include "styx/object.hpp"

#include "styx/list.hpp"
#include "styx/cast.hpp"
#include "styx/get_default.hpp"

#include <iostream>

styx::object::object(const element& o)
{
    const object *m = boost::get<const object>(&o);
    if(m)
        m_map = m->m_map;
}

template<typename T> T styx::object::get(const std::string &k)
{
    typedef typename std::remove_reference<T>::type Tvalue;
    std::map<std::string, styx::element>::iterator it = m_map.find(k);
    if(it != m_map.end())
    {
        styx::element replacement = styx::cast<Tvalue>(it->second);
        m_map[k] = replacement;
        return boost::get<Tvalue&>(m_map[k]);
    }
    m_map[k] = get_default<Tvalue>();
    return boost::get<T>(m_map[k]);
}

template<typename T> T styx::object::copy(const std::string &k) const
{
    std::map<std::string, styx::element>::const_iterator it = m_map.find(k);
    if(it != m_map.end())
        return styx::cast<T>(it->second);
    return get_default<T>();
}

double styx::object::copy_double(const std::string& s) const
{
    return copy<double>(s);
}

styx::int_type styx::object::copy_int(const std::string& s) const
{
    return copy<styx::int_type>(s);
}

styx::list styx::object::copy_list(const std::string& s) const
{
    return copy<list>(s);
}

std::string styx::object::copy_string(const std::string& s) const
{
    return copy<std::string>(s);
}

styx::object styx::object::copy_object(const std::string& s) const
{
    return copy<object>(s);
}

styx::element styx::object::copy_element(const std::string& s) const
{
    return copy<element>(s);
}

bool styx::object::copy_bool(const std::string& s) const
{
    return copy<bool>(s);
}

template bool& styx::object::get<bool&>(const std::string&);
template double& styx::object::get<double&>(const std::string&);
template styx::int_type& styx::object::get<styx::int_type&>(const std::string&);
template std::string& styx::object::get<std::string&>(const std::string&);
template styx::list& styx::object::get<styx::list&>(const std::string&);
template styx::object& styx::object::get<styx::object&>(const std::string&);

template bool styx::object::get<bool>(const std::string&);
template double styx::object::get<double>(const std::string&);
template styx::int_type styx::object::get<styx::int_type>(const std::string&);
template std::string styx::object::get<std::string>(const std::string&);
template styx::list styx::object::get<styx::list>(const std::string&);
template styx::object styx::object::get<styx::object>(const std::string&);

template bool styx::object::copy<bool>(const std::string&) const;
template double styx::object::copy<double>(const std::string&) const;
template styx::int_type styx::object::copy<styx::int_type>(const std::string&) const;
template std::string styx::object::copy<std::string>(const std::string&) const;
template styx::list styx::object::copy<styx::list>(const std::string&) const;
template styx::object styx::object::copy<styx::object>(const std::string&) const;

template<>
styx::element& styx::object::get<styx::element&>(const std::string &k)
{
    return const_cast<styx::object*>(this)->m_map[k];
}

template<>
styx::element styx::object::get<styx::element>(const std::string &k)
{
    return const_cast<styx::object*>(this)->m_map[k];
}

