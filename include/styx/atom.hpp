#ifndef STYX_ATOM_HPP
#define STYX_ATOM_HPP

#include <boost/variant.hpp>

#include "styx/null.hpp"

namespace styx
{
    /*!
     * \brief Any 'atomic' JSON type.  The type cannot be divided into other
     * JSON types.
     */
    typedef boost::variant<
        null_t,
        std::string,
        double,
        int,
        bool>
        atom;
}

#endif

