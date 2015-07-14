#include "styx/parser.hpp"
#include "styx/styx.hpp"

#include <boost/lexical_cast.hpp>
#include <sstream>

#include "styx/exception.hpp"
#include "styx/list.hpp"
#include "styx/object.hpp"

#include "jsmn.h"

namespace
{
    const int start_tokens = 256;
    const int max_tokens = 65536;
}

styx::parser::parser() :
    m_cursor(0),
    m_token_count(start_tokens),
    m_tokens_filled(0),
    m_tokens(new jsmntok_t[m_token_count]),
    m_tok(nullptr)
{
    jsmn_init(&m_parser);
}

void styx::parser::reset()
{
    m_cursor = 0;
    m_tokens_filled = 0;
    jsmn_init(&m_parser);
}

styx::element styx::parser::parse(std::istream& in)
{
    reset();
    std::ostringstream oss;
    for(std::string line; std::getline(in, line);)
        oss << line << "\n";
    std::string data = oss.str();
    return parse(data.c_str(), data.size());
}

styx::element styx::parser::parse(const char *buf, size_t len)
{
    reset();
    m_buf = buf;
    jsmnerr_t err =
        jsmn_parse(&m_parser, m_buf, len, m_tokens.get(), m_token_count);
    if(err == JSMN_ERROR_NOMEM)
    {
        if(m_token_count < max_tokens)
        {
            m_tokens.reset(new jsmntok_t[m_token_count*=2]);
            return parse(buf, len);
        }
        else
            throw exception::no_memory();
    }
    if(err == JSMN_ERROR_INVAL)
        throw exception::invalid_json();
    if(err == JSMN_ERROR_PART)
        throw exception::invalid_json();
    if(err > 0)
    {
        m_tokens_filled = err;
        // There were no errors, proceed to parse the tokens.
        m_tok = &m_tokens[0];
        return parse_token();
    }

    throw exception::base("unknown error");
}

void styx::parser::next_token()
{
    if(m_cursor >= m_tokens_filled)
        throw exception::base("too many tokens requested");
    m_tok = &m_tokens[m_cursor];
    ++m_cursor;
}

jsmntok_t *styx::parser::tok()
{
    return m_tok;
}

styx::element styx::parser::parse_token()
{
    next_token();
    jsmntok_t *this_tok = tok();
    const char *start = m_buf + this_tok->start;
    const char *end = m_buf + this_tok->end;
    switch(this_tok->type)
    {
        case JSMN_PRIMITIVE:
            if(std::string(start, end).compare("true") == 0)
                return true;
            if(std::string(start, end).compare("false") == 0)
                return false;
            // If the primitive contains a '.', assume it is a double.
            if(std::find(start, end, '.') != end)
                try
                {
                    return boost::lexical_cast<double>(std::string(start, end));
                }
                catch(const boost::bad_lexical_cast&)
                {
                    return styx::null_t();
                }

            // Final case, try parsing an int.
            try
            {
                return boost::lexical_cast<styx::int_type>(std::string(start, end));
            }
            catch(const boost::bad_lexical_cast&)
            {
                return styx::null_t();
            }
            break;
        case JSMN_OBJECT:
            {
                styx::object out;
                for(int i = 0; i < this_tok->size; ++i)
                {
                    next_token();
                    jsmntok_t *key = tok();
                    out[std::string(m_buf + key->start, m_buf + key->end)] =
                        parse_token();
                }
                return out;
            }
            break;
        case JSMN_ARRAY:
            {
                styx::list out;
                for(int i = 0; i < this_tok->size; ++i)
                    out.append(parse_token());
                return out;
            }
            break;
        case JSMN_STRING:
            return unescape(
                    std::string(m_buf + this_tok->start, m_buf + this_tok->end)
                    );
    }
    return styx::null_t();
}
