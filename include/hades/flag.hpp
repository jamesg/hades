#ifndef HADES_FLAG_HPP
#define HADES_FLAG_HPP

#include "styx/object_accessor.hpp"
#include "hades/crud.hpp"
#include "hades/relation.hpp"

namespace hades
{
    class connection;
    namespace detail
    {
        struct basic_flag
        {
        };
    }
    template<typename Id, const char *Relation>
    struct flag :
        public detail::basic_flag,
        public Id::candidate_key_type,
        public Id::candidate_key_type::tuple_type,
        public relation<Relation>,
        public crud<flag<Id, Relation>>
    {
        flag(Id id_) :
            styx::object_accessor(id_.get_element())
        {
        }
        bool isset(hades::connection& conn)
        {
            return false;
        }
        //void set(hades::connection& conn)
        //{
            //try
            //{
                //crud<flag<Id, Relation>>::save();
            //}
            //catch(const std::exception&)
            //{
            //}
        //}
        //void unset(hades::connection& conn)
        //{
            //try
            //{
                //crud<flag<Id, Relation>>::destroy();
            //}
            //catch(const std::exception&)
            //{
            //}
        //}
    };
}

#endif

