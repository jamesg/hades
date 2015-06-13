#ifndef STYX_ELEMENT_HPP
#define STYX_ELEMENT_HPP

#include <boost/variant.hpp>

#include "styx/null.hpp"

namespace styx
{
    class list;
    class object;

    typedef boost::recursive_wrapper<object> r_object;
    typedef boost::recursive_wrapper<list> r_list;

    /*!
     * \brief The JSON data storage type.  Represents any type of JSON data
     * (object, array, string, number, bool or null).
     *
     * \note An element will always be initialised to null.
     */
    typedef boost::variant<
        null_t,
        r_object,
        std::string,
        r_list,
        double,
        int,
        bool>
        element;

    /*!
     * \brief Determine whether the given element is null.
     */
    bool is_null(const styx::element&);
}

#endif

