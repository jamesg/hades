#include "styx/styx.hpp"

#include <fstream>
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

styx::element styx::parse_json(std::istream& in)
{
    parser p;
    return p.parse(in);
}

styx::element styx::parse_json_file(const std::string& filename)
{
    std::ifstream is(filename);
    return parse_json(is);
}
