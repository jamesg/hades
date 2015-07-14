#ifndef STYX_HPP
#define STYX_HPP

#include <string>

#include "styx/element.hpp"
#include "styx/list.hpp"
#include "styx/object.hpp"

namespace styx
{
    std::string escape(std::string);
    std::string unescape(std::string);
    /*!
     * \brief Parse a JSON string into a styx::element.  The JSON string may be
     * any element (object, string, list etc.).
     *
     * \returns True if the parse was successful, false otherwise.
     * \throws Nothing.
     */
    styx::element parse_json(const std::string&);

    // Parse the input from an input stream.  The stream is read until it is
    // empty.
    styx::element parse_json(std::istream&);

    // Open a file and parse the contents.
    styx::element parse_json_file(const std::string& filename);
}

#endif
