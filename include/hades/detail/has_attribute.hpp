#ifndef HADES_DETAIL_HAS_ATTRIBUTE_HPP
#define HADES_DETAIL_HAS_ATTRIBUTE_HPP

namespace hades
{
    namespace detail
    {
        /*!
         * \internal
         * \brief Establish the fact that a type has an attribute.  Used to
         * enable accessor functions when the type has the required attribute.
         */
        template<const char *Attribute>
        struct has_attribute
        {
        };
    }
}

#endif

