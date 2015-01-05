#ifndef STYX_SERIALISE_JSON_HPP
#define STYX_SERIALISE_JSON_HPP

#include <string>

#include "styx/element.hpp"

namespace styx
{
    /*!
     * \brief Serialise a data structure as JSON.  Rules on adding whitespace
     * and newlines are not defined.
     */
    std::string serialise_json(const element&);
}

#endif

