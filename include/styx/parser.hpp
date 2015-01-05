#ifndef STYX_PARSER_HPP
#define STYX_PARSER_HPP

#include <boost/scoped_array.hpp>

#include "jsmn.h"

#include "styx/element.hpp"

namespace styx
{
    class parser
    {
        public:
            parser();

            styx::element parse(const char *buf, size_t len);

        private:
            void reset();
            /*!
             * \brief Advance the current token pointer to the next token.
             * \throws styx::exception::base if there are no more tokens to
             * parse.
             */
            void next_token();
            /*!
             * \brief Get the token currently being parsed.
             */
            jsmntok_t *tok();
            /*!
             * \brief Completely parse the current token (as returned by
             * tok()).
             */
            styx::element parse_token();

            /*!
             * \brief Input string to parse.  After JSMN has tokenised the
             * input, it is used by parse_token to fill styx::element's.
             */
            const char *m_buf;

            size_t m_cursor;
            size_t m_token_count;
            size_t m_tokens_filled;
            boost::scoped_array<jsmntok_t> m_tokens;
            jsmn_parser m_parser;
            jsmntok_t *m_tok;
    };
}

#endif

