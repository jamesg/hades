/*
 * JSON - a JSON data structure, parser and serialiser library.
 * Copyright (C) 2014 James Goode.
 */

#include "styx/serialisers/string.hpp"

#include <sstream>

void styx::serialise(const std::string &s, std::ostream &os)
{
    std::istringstream is(s);
    std::string line;
    if( std::getline(is, line) )
        os << line;
    while( std::getline(is, line) )
        os << "\\n" << line;
}

