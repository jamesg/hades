#ifndef STYX_NULL_HPP
#define STYX_NULL_HPP

namespace styx
{
    /*!
     * \brief Dummy type for the 'null' STYX value (boost::variant doesn't
     * allow null values otherwise).
     */
    class null_t {
    public:
        null_t()
        {
        }

        template <typename T, typename U>
        null_t(const T&, const U&)
        {
        }

        template <typename T>
        explicit null_t(const T&)
        {
        }
    };
    typedef null_t null;
}

#endif

