#ifndef STYX_GET_DEFAULT_HPP
#define STYX_GET_DEFAULT_HPP

#include "styx/element.hpp"

namespace styx
{
    /*!
     * \brief Get the 'default' value for a JSON type.
     *
     * The default value will be empty for a container or 0 for a number.
     */
    template <typename T> T get_default() { return T(); }

    template<> int get_default<int>();
    template<> double get_default<double>();
    template<> bool get_default<bool>();
    template<> element get_default<element>();
}

#endif

