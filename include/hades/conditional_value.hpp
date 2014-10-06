#ifndef HADES_CONDITIONAL_VALUE_HPP
#define HADES_CONDITIONAL_VALUE_HPP

#include <type_traits>

namespace hades
{
    /*!
     * \brief Expose a static const variable of type Result.  The value of the
     * variable is determined by a boolean template argument.
     *
     * \param Result Type of the exposed variable.  Must be an integral type to
     * be used as a static const member.
     * \param C Condition deciding between T and F.
     * \param T Value to expose if C == true.
     * \param F Value to expose if C == false.
     */
    template<typename Result, bool C, Result T, Result F>
    using conditional_value =
            typename std::conditional<
                C,
                std::integral_constant<Result, T>,
                std::integral_constant<Result, F>
                >::type;
}

#endif

