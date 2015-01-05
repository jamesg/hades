#include "styx/styx.hpp"

#include <sstream>
#include <stdexcept>

#include <boost/regex.hpp>

#include "styx/list.hpp"
#include "styx/object.hpp"
#include "styx/parser.hpp"

std::string styx::escape(std::string s)
{
    try
    {
        boost::regex e(std::string("\""));
        return boost::regex_replace(s, e, std::string("\\\\\\\""));
    }
    catch(const std::exception &e)
    {
    }
    return "";
}

std::string styx::unescape(std::string s)
{
    boost::regex e(std::string("\\\\(.)"));
    return boost::regex_replace(s, e, std::string("$1"));
}

styx::element styx::parse_json(const std::string& str)
{
    parser p;
    return p.parse(str.c_str(), str.length());
}

