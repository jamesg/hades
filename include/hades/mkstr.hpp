#ifndef HADES_MKSTR_HPP
#define HADES_MKSTR_HPP

#include <sstream>

namespace hades
{
    /*!
     * \brief Helper class to construct a std::ostringstream, stream data to
     * it, and convert it to a string.
     *
     * mkstr objects are implicitly convertable to strings.  Any type that can
     * be streamed to a std::ostringstream can be streamed to an mkstr.
     *
     * \code
     * std::string s = (hades::mkstr() << "A" << 1);
     * \endcode
     */
    class mkstr
    {
        public:
            template<typename Output>
            mkstr& operator<<(const Output& output)
            {
                m_oss << output;
                return *this;
            }
            operator std::string() const
            {
                return str();
            }
            std::string str() const
            {
                return m_oss.str();
            }
        private:
            std::ostringstream m_oss;
    };
}

#endif

