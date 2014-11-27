#include "hades/filter.hpp"

hades::detail::where<hades::row<>> hades::where()
{
    return hades::detail::where<hades::row<>>();
}

hades::detail::where<hades::row<>> hades::where(const std::string& clause_)
{
    return hades::detail::where<hades::row<>>(clause_);
}

