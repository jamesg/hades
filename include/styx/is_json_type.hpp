#ifndef STYX_IS_JSON_TYPE_HPP
#define STYX_IS_JSON_TYPE_HPP

#include "styx/element.hpp"

namespace styx
{
    /*!
     * \brief Check whether a type is a fundamental JSON type.  These are
     * styx::element and any type that can be stored in a styx::element.
     */
    template<typename Type>
    struct is_json_type
    {
        static constexpr bool value = false;
    };

    template<>
    struct is_json_type<bool>
    {
        static constexpr bool value = true;
    };

    template<>
    struct is_json_type<double>
    {
        static constexpr bool value = true;
    };

    template<>
    struct is_json_type<element>
    {
        static constexpr bool value = true;
    };

    template<>
    struct is_json_type<int>
    {
        static constexpr bool value = true;
    };

    template<>
    struct is_json_type<list>
    {
        static constexpr bool value = true;
    };

    template<>
    struct is_json_type<object>
    {
        static constexpr bool value = true;
    };

    template<>
    struct is_json_type<std::string>
    {
        static constexpr bool value = true;
    };
}

#endif

