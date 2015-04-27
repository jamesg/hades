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
        typedef std::map<std::string, element> map_type;
        typedef map_type::size_type size_type;
        typedef map_type::iterator iterator;
        typedef map_type::const_iterator const_iterator;
        typedef map_type::reverse_iterator reverse_iterator;
        typedef map_type::const_reverse_iterator const_reverse_iterator;

        object()
        {
        }
        /*!
         */
        object(const element& o);

        // Iterators.
        iterator begin() { return m_map.begin(); }
        iterator end() { return m_map.end(); }
        const_iterator cbegin() const { return m_map.cbegin(); }
        const_iterator cend() const { return m_map.cend(); }
        reverse_iterator rbegin() { return m_map.rbegin(); }
        reverse_iterator rend() { return m_map.rend(); }
        const_reverse_iterator crbegin() const { return m_map.crbegin(); }
        const_reverse_iterator crend() const { return m_map.crend(); }

        element& operator[](const std::string &k)
        {
            return m_map[k];
        }

        bool has_key(const std::string& key) const
        {
            return (m_map.find(key) != m_map.cend());
        }

        std::map<std::string, element>::size_type erase(const std::string& key)
        {
            return m_map.erase(key);
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

        friend std::string serialise_json(const element&);

        map_type m_map;
    };

    template<>
    element& object::get<styx::element&>(const std::string&);
    template<>
    element object::get<styx::element>(const std::string&);
}

#endif

