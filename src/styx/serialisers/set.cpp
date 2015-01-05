/*
 * JSON - a JSON data structure, parser and serialiser library.
 * Copyright (C) 2014 James Goode.
 */

#include <sstream>

#include "styx/serialisers/set.hpp"

std::string styx::implode_string( const std::set<std::string>& v,
        std::string sep )
{
    std::ostringstream oss;
    serialise(
            v,
            []( const std::string &s, std::ostream &os )
            {
                os << s;
            },
            sep,
            oss );
    return oss.str();
}

