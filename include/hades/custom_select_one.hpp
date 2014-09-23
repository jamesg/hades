#ifndef HADES_CUSTOM_SELECT_ONE_HPP
#define HADES_CUSTOM_SELECT_ONE_HPP

#include "styx/list.hpp"

#include "hades/custom_select.hpp"

namespace hades
{
    /*!
     * \brief Execute a custom SELECT statement that is expected to return
     * exactly one tuple.  An exception is thrown if xero or more than one
     * tuples are returned.
     */
    template<typename Tuple, typename Values, const char *...Attributes>
    styx::element custom_select_one(
            hades::connection& conn,
            const std::string& query,
            const Values& values
            )
    {
        styx::list out = custom_select<Tuple, Values, Attributes...>(
                conn,
                query,
                values
                );
        if(out.size() == 0)
            throw std::runtime_error("no row in custom_select_one");
        else if(out.size() > 1)
            throw std::runtime_error("more than one row in custom_select_one");
        else
            return out[0];
    }
}

#endif

