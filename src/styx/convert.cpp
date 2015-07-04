#include "styx/convert.hpp"

#include <boost/lexical_cast.hpp>

#include "styx/get_default.hpp"
#include "styx/list.hpp"
#include "styx/object.hpp"
#include "styx/styx.hpp"

namespace
{
    class convert_visitor : public boost::static_visitor<bool>
    {
        public:
        bool operator()(const double& from, double& to) const
        {
            to = from;
            return true;
        }
        bool operator()(const double& from, styx::int_type& to) const
        {
            to = static_cast<styx::int_type>(from);
            return true;
        }
        bool operator()(const double& from, std::string& to) const
        {
            std::ostringstream oss;
            oss << from;
            to = oss.str();
            return true;
        }
        bool operator()(const double&, styx::object&) const
        {
            return false;
        }
        bool operator()(const double&, styx::list&) const
        {
            return false;
        }
        bool operator()(const double& from, styx::null_t& to) const
        {
            to = styx::get_default<styx::null_t>();
            return true;
        }
        bool operator()(const double& from, bool& to) const
        {
            return false;
        }

        bool operator()(const styx::int_type& from, double& to) const
        {
            to = static_cast<styx::int_type>(from);
            return true;
        } //< loss of precision
        bool operator()(const styx::int_type& from, styx::int_type& to) const
        {
            to = from;
            return true;
        }
        bool operator()(const styx::int_type& from, std::string& to) const
        {
            std::ostringstream oss;
            oss << from;
            to = oss.str();
            return true;
        }
        bool operator()(const styx::int_type&, styx::object&) const
        {
            return false;
        }
        bool operator()(const styx::int_type&, styx::list&) const
        {
            return false;
        }
        bool operator()(const styx::int_type& from, styx::null_t& to) const
        {
            to = styx::get_default<styx::null_t>();
            return true;
        }
        bool operator()(const styx::int_type& from, bool& to) const
        {
            to = (from > 0);
            return true;
        }

        bool operator()(const std::string& from, styx::int_type& to) const
        {
            to = boost::lexical_cast<styx::int_type>(from);
            return true;
        }
        bool operator()(const std::string& from, double& to) const
        {
            to = boost::lexical_cast<double>(from);
            return true;
        }
        bool operator()(const std::string& from, std::string& to) const
        {
            to = from;
            return true;
        }
        bool operator()(const std::string&, styx::object&) const
        {
            return false;
        }
        bool operator()(const std::string& from, styx::list& to) const {
            if( from.length() == 0 ) {
                to = styx::list();
                return true;
            } else {
                return false;
            }
        }
        bool operator()(const std::string& from, styx::null_t& to) const
        {
            to = styx::get_default<styx::null_t>();
            return true;
        }
        bool operator()(const std::string& from, bool& to) const
        {
            return false;
        }

        bool operator()(const styx::object&, styx::int_type&) const
        {
            return false;
        }
        // Converting from a map to a string is allowed only if the map is
        // empty.
        bool operator()(const styx::object& from, std::string& to) const
        {
            if(from.m_map.size() == 0) {
                to = "";
                return true;
            } else {
                return false;
            }
        }
        bool operator()(const styx::object&, double&) const
        {
            return false;
        }
        bool operator()(const styx::object& from, styx::object& to) const
        {
            to = from;
            return true;
        }
        // Converting from a map to a list is allowed only if the map is empty.
        bool operator()(const styx::object& from, styx::list& to) const
        {
            if(from.m_map.size() == 0) {
                to = styx::list();
                return true;
            } else {
                return false;
            }
        }
        bool operator()(const styx::object& from, styx::null_t& to) const
        {
            to = styx::get_default<styx::null_t>();
            return true;
        }
        bool operator()(const styx::object& from, bool& to) const
        {
            return false;
        }

        bool operator()(const styx::list&, styx::int_type&) const
        {
            return false;
        }
        bool operator()(const styx::list&, std::string&) const
        {
            return false;
        }
        bool operator()(const styx::list& from, styx::object&) const
        {
            return from.size() == 0;
        }
        bool operator()(const styx::list&, double&) const
        {
            return false;
        }
        bool operator()(const styx::list& from, styx::list& to) const
        {
            to = from; return true;
        }
        bool operator()(const styx::list& from, styx::null_t& to) const
        {
            to = styx::get_default<styx::null_t>();
            return true;
        }
        bool operator()(const styx::list& from, bool& to) const
        {
            return false;
        }

        bool operator()(const styx::null_t&, double& to) const
        {
            to = styx::get_default<double>();
            return true;
        }
        bool operator()(const styx::null_t&, styx::int_type& to) const
        {
            to = styx::get_default<styx::int_type>();
            return true;
        }
        bool operator()(const styx::null_t&, std::string& to) const
        {
            to = styx::get_default<std::string>();
            return true;
        }
        bool operator()(const styx::null_t&, styx::object& to) const
        {
            to = styx::get_default<styx::object>();
            return true;
        }
        bool operator()(const styx::null_t&, styx::list& to) const
        {
            to = styx::get_default<styx::list>();
            return true;
        }
        bool operator()(const styx::null_t& from, styx::null_t& to) const
        {
            to = from;
            return true;
        }
        bool operator()(const styx::null_t& from, bool& to) const
        {
            to = false;
            return true;
        }

        bool operator()(const bool& from, double& to) const
        {
            return false;
        }
        bool operator()(const bool& from, styx::int_type& to) const
        {
            return false;
        }
        bool operator()(const bool& from, std::string& to) const
        {
            return false;
        }
        bool operator()(const bool& from, styx::object& to) const
        {
            return false;
        }
        bool operator()(const bool& from, styx::list& to) const
        {
            return false;
        }
        bool operator()(const bool& from, styx::null_t& to) const
        {
            to = styx::get_default<styx::null_t>();
            return true;
        }
        bool operator()(const bool& from, bool& to) const
        {
            to = from;
            return true;
        }
    };
}

bool styx::convert(const element& from, element& to)
{
    return boost::apply_visitor(convert_visitor(), from, to);
}

