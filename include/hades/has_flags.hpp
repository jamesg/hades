#ifndef HADES_FLAGS_HPP
#define HADES_FLAGS_HPP

#include "hades/crud/save.ipp"
#include "hades/detail/basic_has_flags.hpp"
#include "hades/detail/has_attribute.hpp"
#include "hades/flag.hpp"
#include "styx/object_accessor.hpp"

namespace hades
{
    namespace detail
    {
        template<const char *Flag>
        struct has_flag :
            public has_attribute<Flag>,
            virtual styx::object_accessor
        {
            template<typename Tuple>
            void save_flag(Tuple& t, connection& conn)
            {
                std::cerr << "hades saving flag " << Tuple::relation_name << std::endl;
                bool flag_value = get_bool(Flag);
                flag<typename Tuple::id_type, Flag> f(t.id());
                if(flag_value)
                    save(f, conn);
                else
                    destroy(f, conn);
            }
        };
    }
    template<const char *...Keys>
    struct has_flags :
        public detail::has_flag<Keys>...,
        public detail::basic_has_flags,
        virtual styx::object_accessor
    {
        template<typename Tuple>
        void save_flags(Tuple& t, connection& conn)
        {
            save_flags_<Tuple, Keys...>(t, conn);
        }
    private:
        template<typename Tuple, const char *Flag>
        void save_flags_(Tuple& t, connection& conn)
        {
            detail::has_flag<Flag>::save_flag(t, conn);
        }
        template<typename Tuple, const char *Flag1, const char *Flag2, const char *...Flags>
        void save_flags_(Tuple& t, connection& conn)
        {
            save_flags_<Tuple, Flag1>(t, conn);
            save_flags_<Tuple, Flag2, Flags...>(t, conn);
        }
    };
}

#endif

