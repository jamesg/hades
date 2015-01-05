#ifndef SERIALISERS_VECTOR_HPP
#define SERIALISERS_VECTOR_HPP

#include <vector>
#include <ostream>
#include <string>

namespace styx
{
    template <typename Func, typename T>
    void serialise(
            const std::vector<T> &v,
            Func f,
            std::string sep,
            std::ostream &os);

    std::string implode_string(const std::vector<std::string>&, std::string);
}

template <typename Func, typename T>
void styx::serialise(
        const std::vector<T> &v,
        Func f,
        std::string sep,
        std::ostream &os)
{
    typename std::vector<T>::const_iterator i = v.begin();
    while(i != v.end())
    {
        f(*i, os);
        ++i;
        if(i != v.end())
            os << sep;
    }
}

#endif

