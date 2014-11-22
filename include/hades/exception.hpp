#ifndef HADES_EXCEPTION_HPP
#define HADES_EXCEPTION_HPP

#include <stdexcept>

namespace hades
{
    /*!
     * \brief Generic exception.  Thrown for all database exceptions generated
     * by the Hades library.
     */
    class exception :
        public std::runtime_error
    {
        public:
            exception(const std::string& message) :
                std::runtime_error(message)
            {
            }
    };
}

#endif

