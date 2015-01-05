#ifndef STYX_CAST_HPP
#define STYX_CAST_HPP

#include <sstream>
#include <stdexcept>

#include "styx/element.hpp"

namespace styx
{
    /*!
     * \brief Cast a JSON value to a compatible type, or throw an exception if
     * the target type is incompatible.
     */
    template<typename Tto>
    Tto cast(const styx::element& from);
    template<>
    styx::element cast<styx::element>(const styx::element& from);
}

#endif

