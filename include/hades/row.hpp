#ifndef HADES_ROW_HPP
#define HADES_ROW_HPP

#include <vector>

#include <boost/fusion/include/at_c.hpp>
#include <boost/fusion/include/vector.hpp>

namespace hades
{
    /*!
     * Convenience type for representing a row without having to use a Boost
     * Fusion adapted struct.
     *
     * Currently an alias for a boost::fusion::vector.
     */
    template<typename ...COLUMNS> using row =
        boost::fusion::vector<COLUMNS...>;

    /*!
     * Get the value of a column from a row.  The index of the column is a
     * zero-based integer provided as the first template argument.
     *
     * This function is an alias for boost::fusion::at_c, which provides
     * exactly the required functionality for a boost::fusion::vector (aliased
     * as hades::row).
     */
    template <int N, typename SEQUENCE>
    inline typename boost::fusion::result_of::at_c<SEQUENCE const, N>::type
    column(SEQUENCE const& seq)
    {
        return boost::fusion::at<boost::mpl::int_<N> >(seq);
    }

    /*!
     * Get an empty row object.  Useful when no parameters are required for a
     * query but the parameter argument is still required.
     */
    hades::row<> empty_row();

    /*!
     * A set of rows represents the result of a SELECT query.
     *
     * The name is chosen because the type is analogous to the ROWSET type in
     * some dialects of SQL.  Currently an alias for a
     * std::vector<hades::row>.
     */
    template<typename ...COLUMNS> using rowset =
        std::vector<row<COLUMNS...>>;
}

#endif

