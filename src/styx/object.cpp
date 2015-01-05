#include "styx/object.hpp"

#include "styx/list.hpp"
#include "styx/cast.hpp"
#include "styx/get_default.hpp"

styx::object::object(const element& o)
{
    object *m = boost::get<object>(&const_cast<element&>(o));
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

std::string styx::object::copy_string(const std::string& s) const
{
    return copy<std::string>(s);
}

template bool& styx::object::get<bool&>(const std::string&);
template double& styx::object::get<double&>(const std::string&);
template int& styx::object::get<int&>(const std::string&);
template std::string& styx::object::get<std::string&>(const std::string&);
template styx::list& styx::object::get<styx::list&>(const std::string&);
template styx::object& styx::object::get<styx::object&>(const std::string&);

template bool styx::object::get<bool>(const std::string&);
template double styx::object::get<double>(const std::string&);
template int styx::object::get<int>(const std::string&);
template std::string styx::object::get<std::string>(const std::string&);
template styx::list styx::object::get<styx::list>(const std::string&);
template styx::object styx::object::get<styx::object>(const std::string&);

template bool styx::object::copy<bool>(const std::string&) const;
template double styx::object::copy<double>(const std::string&) const;
template int styx::object::copy<int>(const std::string&) const;
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

