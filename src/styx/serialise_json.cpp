#include "styx/serialise_json.hpp"
#include "styx/styx.hpp"

#include <sstream>

#include "styx/list.hpp"
#include "styx/object.hpp"
#include "styx/serialisers/deque.hpp"
#include "styx/serialisers/map.hpp"
#include "styx/serialisers/vector.hpp"
#include "styx/serialisers/string.hpp"

std::string styx::serialise_json(const element &jobj)
{
    class serialise_visitor : public boost::static_visitor<void>
    {
        public:
        serialise_visitor(std::ostream &os) :
            output_stream(os)
        {
        }

        void operator()(const std::string& str) const
        {
            output_stream << "\"";
            serialise(escape(str), output_stream);
            output_stream << "\"";
        }

        void operator()(const r_object& object) const
        {
            output_stream << "{ ";
            styx::serialise(
                    object.get().m_map,
                    [](const std::string &s, std::ostream &os)
                    {
                        os << "\"" << escape(s) << "\"";
                    },
                    [](const styx::element &o, std::ostream &os)
                    {
                        boost::apply_visitor(serialise_visitor(os), o);
                    },
                    " : ",
                    ", ",
                    output_stream
                    );
            output_stream << " }";
        }

        void operator()(const r_list& list) const
        {
            output_stream << "[ ";
            styx::serialise(
                list.get().elements,
                [](const styx::element &o, std::ostream &os) {
                    boost::apply_visitor(serialise_visitor(os), o);
                },
                ", ",
                output_stream
                );
            output_stream << " ]";
        }

        void operator()(const double& d) const
        {
            std::ostringstream oss;
            oss << d;
            output_stream << oss.str();
        }

        void operator()(const int& d) const
        {
            std::ostringstream oss;
            oss << d;
            output_stream << oss.str();
        }

        void operator()(const null_t&) const
        {
            output_stream << "null";
        }

        void operator()(const bool& b) const
        {
            output_stream << (b?"true":"false");
        }

        private:
        std::ostream &output_stream;
    };

    std::ostringstream oss;
    boost::apply_visitor(serialise_visitor(oss), jobj);

    return oss.str();
}

