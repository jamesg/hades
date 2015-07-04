#include "styx/cast.hpp"

#include "styx/convert.hpp"
#include "styx/get_default.hpp"
#include "styx/list.hpp"
#include "styx/object.hpp"
#include "styx/serialise_json.hpp"

template<typename Tto>
Tto styx::cast(const styx::element& from)
{
    styx::element to = get_default<Tto>();
    if(!convert(from, to))
    {
        std::ostringstream oss;
        oss << "invalid styx cast from " << serialise_json(from) <<
            " to " << serialise_json(to);
        throw std::runtime_error(oss.str());
    }

    try
    {
        return boost::get<Tto>(to);
    }
    catch(const std::exception&)
    {
        std::ostringstream oss;
        oss << "styx::cast: casting " << serialise_json(from) << " to " << serialise_json(to);
        throw std::runtime_error(oss.str());
    }
}

template styx::list styx::cast<styx::list>(const styx::element&);
template styx::object styx::cast<styx::object>(const styx::element&);
template bool styx::cast<bool>(const styx::element&);
template double styx::cast<double>(const styx::element&);
template styx::int_type styx::cast<styx::int_type>(const styx::element&);
template std::string styx::cast<std::string>(const styx::element&);

template styx::element styx::cast<styx::element>(const styx::element&);

template<> styx::element styx::cast<styx::element>(const styx::element& from)
{
    return from;
}

