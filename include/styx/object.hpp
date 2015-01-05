#ifndef STYX_OBJECT_HPP
#define STYX_OBJECT_HPP

#include <map>
#include <string>

#include "styx/element.hpp"

namespace styx
{
    /*!
     * \brief A JSON object (map of strings to JSON elements).
     *
     * \note Represented using a C++ map.
     */
    struct object
    {
        object()
        {
        }
        /*!
         */
        object(const element& o);

        element& operator[](const std::string &k)
        {
            return m_map[k];
        }

        bool has_key(const std::string& key) const
        {
            return (m_map.find(key) != m_map.cend());
        }

        // Variant accessor functions - may throw a boost::bad_get exception.

        // Generic 'get' function.
        template<typename T> T get(const std::string& k);

        template<typename T> T copy(const std::string& k) const;

        std::string copy_string(const std::string& k) const;

        double& get_double(const std::string &k)
        {
            return get<double&>(k);
        }
        int& get_int(const std::string &k)
        {
            return get<int&>(k);
        }
        list& get_list(const std::string &k)
        {
            return get<list&>(k);
        }
        std::string& get_string(const std::string &k)
        {
            return get<std::string&>(k);
        }
        object& get_object(const std::string &k)
        {
            return get<object&>(k);
        }
        element& get_element(const std::string &k)
        {
            return m_map[k];
        }
        bool& get_bool(const std::string& k)
        {
            return get<bool&>(k);
        }

        //void insert(const std::pair<std::string, styx::element>& p)
        //{
            //m_map.insert(p);
        //}

        friend std::string serialise_json(const element&);

        std::map<std::string, element> m_map;
    };

    template<>
    element& object::get<styx::element&>(const std::string&);
    template<>
    element object::get<styx::element>(const std::string&);
}

#endif

