#ifndef HADES_HAS_FLAGS_HPP
#define HADES_HAS_FLAGS_HPP

#ifdef HADES_ENABLE_DEBUGGING
#include <iostream>
#endif
#include "hades/detail/basic_has_flags.hpp"
#include "hades/detail/has_attribute.hpp"
#include "hades/flag.hpp"
#include "styx/object.hpp"

namespace hades
{
    namespace detail
    {
        /*!
         * \brief Annotate a tuple with a single boolean flag.
         * \param Flag Relvar to use to store the flag.
         *
         * The boolean flag will be recorded as the presence or absence of a
         * tuple with the same id as a the related tuple.  The flag will be
         * stored in a relvar named 'Flag'.  The relvar should have been
         * created with the same attributes as the tuple's candidate key.
         */
        template<const char *Flag>
        struct has_flag :
            public has_attribute<Flag>,
            virtual styx::object
        {
            template<typename Tuple>
            void save_flag(Tuple& t, connection& conn)
            {
#ifdef HADES_ENABLE_DEBUGGING
                std::cerr << "hades saving flag " << Tuple::relation_name << std::endl;
#endif
                bool flag_value = get_bool(Flag);
                flag<typename Tuple::id_type, Flag> f(t.id());
                try
                {
                    if(flag_value)
                        f.save(conn);
                    else
                        f.destroy(conn);
                }
                catch(const std::exception&)
                {
                }
            }
            template<typename Tuple>
            void get_flag(connection& conn, Tuple& t)
            {
#ifdef HADES_ENABLE_DEBUGGING
                std::cerr << "hades getting flag " << Tuple::relation_name << std::endl;
#endif
                flag<typename Tuple::id_type, Flag> f(t.id());
                get_bool(Flag) = f.exists(conn);
            }
        };
    }
    /*!
     * \brief Annotate a tuple with a set of flags, each stored in its own
     * relvar.
     *
     * \param Keys List of relvars that hold the flags.  Each relvar should
     * have attributes matching the candidate key of the tuple type that the
     * flags refer to.
     */
    template<const char *...Keys>
    struct has_flags :
        public detail::has_flag<Keys>...,
        public detail::basic_has_flags,
        virtual styx::object
    {
        template<typename Tuple>
        void get_flags(connection& conn, Tuple& t)
        {
            get_flags_<Tuple, Keys...>(conn, t);
        }
        template<typename Tuple>
        void save_flags(Tuple& t, connection& conn)
        {
            save_flags_<Tuple, Keys...>(t, conn);
        }
    private:
        template<typename Tuple, const char *Flag>
        void get_flags_(connection& conn, Tuple& t)
        {
            detail::has_flag<Flag>::get_flag(conn, t);
        }
        template<typename Tuple, const char *Flag1, const char *Flag2, const char *...Flags>
        void get_flags_(connection& conn, Tuple& t)
        {
            get_flags_<Tuple, Flag1>(conn, t);
            get_flags_<Tuple, Flag2, Flags...>(conn, t);
        }
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

