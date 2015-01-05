#ifndef STYX_CONVERT_HPP
#define STYX_CONVERT_HPP

#include "styx/element.hpp"

namespace styx
{
    /*!
     * \brief Convert a JSON element to one of another type.
     *
     * Conversions are designed to be transparent and reversible.  For example,
     * 3.14 (float) can be converted to "3.14" (string) and vice versa.  Empty
     * structures (object and list) and null can be converted to any type as no
     * information is lost.
     */
    bool convert(const element& from, element& to);
}

#endif

