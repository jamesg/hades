#ifndef HADES_WHERE_HPP
#define HADES_WHERE_HPP

#include "hades/bind_values.hpp"
#include "hades/row.hpp"

namespace hades
{
    template<typename ...Values>
    class where
    {
        public:
            /*!
             * \brief A where clause which selects all tuples.
             */
            where() :
                m_clause("1")
            {
                static_assert(
                        sizeof...(Values) == 0,
                        "a where clause without any placeholders can't have "
                        "any bound values"
                        );
            }
            where(const std::string& clause_, hades::row<Values...> values) :
                m_clause(clause_),
                m_values(values)
            {
            }
            where(const std::string& clause_) :
                m_clause(clause_)
            {
                static_assert(
                        sizeof...(Values) == 0,
                        "a where clause without any placeholders can't have "
                        "any bound values"
                        );
            }
            void bind(sqlite3_stmt *stmt) const
            {
                bind_values(m_values, stmt);
            }
            const std::string& clause() const
            {
                return m_clause;
            }
        private:
            const std::string m_clause;
            hades::row<Values...> m_values;
    };
}

#endif

