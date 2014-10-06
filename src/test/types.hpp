#ifndef HADES_TEST_TYPES_HPP
#define HADES_TEST_TYPES_HPP

#include "hades/crud.hpp"
#include "hades/flag.hpp"
#include "hades/has_candidate_key.hpp"
#include "hades/relation.hpp"
#include "hades/tuple.hpp"
#include "styx/object_accessor.hpp"

namespace hades
{
    class connection;

    namespace test
    {
        namespace attr
        {
            extern const char site_id[];
            extern const char device_id[];
            extern const char city[];
            extern const char name[];
        }

        namespace flag
        {
            namespace site
            {
                extern const char enabled[];
            }
            namespace device
            {
                extern const char enabled[];
            }
        }

        namespace relvar
        {
            extern const char site[];
            extern const char site_location[];
            extern const char device[];
        }

        struct site :
            public hades::relation<relvar::site>,
            public hades::tuple<attr::site_id, attr::name>,
            public hades::has_candidate_key<attr::site_id>,
            public hades::crud<site>
        {
            site()
            {
            }

            site(styx::element& o) :
                styx::object_accessor(o)
            {
            }

            int& site_id()
            {
                return key<attr::site_id>();
            }

            std::string& name()
            {
                return get_string<attr::name>();
            }
        };

        typedef hades::flag<site, test::flag::site::enabled> site_enabled;

        struct site_location :
            public hades::relation<relvar::site_location>,
            public hades::tuple<attr::site_id, attr::city>,
            public hades::has_candidate_key<attr::site_id>,
            public hades::crud<site_location>
        {
            site_location()
            {
            }
            site_location(const styx::element& o) :
                styx::object_accessor(o)
            {
            }
            std::string& city()
            {
 ;               return get_string<attr::city>();
            }
        };

        struct device :
            public hades::relation<relvar::device>,
            public hades::tuple<attr::site_id, attr::device_id, attr::name>,
            public hades::has_candidate_key<attr::site_id, attr::device_id>,
            public hades::crud<device>
        {
            device()
            {
            }

            device(styx::element& o) :
                styx::object_accessor(o)
            {
            }

            int& site_id()
            {
                return key<attr::site_id>();
            }

            int& device_id()
            {
                return key<attr::device_id>();
            }

            std::string& name()
            {
                return get_string<attr::name>();
            }
        };

        typedef hades::flag<device, test::flag::device::enabled> device_enabled;

        void create(hades::connection&);
    }
}

#endif

