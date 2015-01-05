#ifndef STYX_EXCEPTION_HPP
#define STYX_EXCEPTION_HPP

#include <stdexcept>

namespace styx
{
    namespace exception
    {
        /*!
         * \brief Generic exception class, from which all exception classes in
         * Styx derive.
         */
        class base :
            public std::runtime_error
        {
            public:
                base(const std::string& message) :
                    std::runtime_error(message)
                {
                }
        };
        /*!
         * \brief The JSON input was invalid and will not be parsed.
         */
        class invalid_json :
            public base
        {
            public:
                invalid_json() :
                    base("invalid json input")
                {
                }
        };
        /*!
         * \brief The parser has run out of memory and will not allocate more.
         * This exception occurs if the input string is extremely large.
         */
        class no_memory :
            public base
        {
            public:
                no_memory() :
                    base("parser out of memory")
                {
                }
        };
    }
}

#endif

