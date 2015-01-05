#ifndef STYX_SERIALISERS_SET_HPP
#define STYX_SERIALISERS_SET_HPP

#include <set>
#include <ostream>
#include <string>

namespace styx
{
    template <typename Func, typename T>
    void serialise(
            const std::set<T> &v,
            Func f,
            std::string sep,
            std::ostream &os);

    std::string implode_string(const std::set<std::string>&, std::string);
}

template <typename Func, typename T>
void styx::serialise(
        const std::set<T> &v,
        Func f,
        std::string sep,
        std::ostream &os)
{
    typename std::set<T>::const_iterator i = v.begin();
    while(i != v.end())
    {
        f(*i, os);
        ++i;
        if(i != v.end())
            os << sep;
    }
}

#endif

