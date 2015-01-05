#ifndef STYX_SERIALISERS_DEQUE_HPP
#define STYX_SERIALISERS_DEQUE_HPP

#include <deque>
#include <ostream>
#include <string>

namespace styx
{
    template <typename Func, typename T>
    void serialise(
            const std::deque<T> &d,
            Func f,
            std::string sep,
            std::ostream &os );
}

template <typename Func, typename T>
void styx::serialise(
        const std::deque<T> &v,
        Func f,
        std::string sep,
        std::ostream &os
        )
{
    typename std::deque<T>::const_iterator i = v.begin();
    while(i != v.end())
    {
        f(*i, os);
        ++i;
        if(i != v.end())
            os << sep;
    }
}

#endif

