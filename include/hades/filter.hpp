#ifndef HADES_FILTER_HPP
#define HADES_FILTER_HPP

#include "hades/bind_values.hpp"
#include "hades/mkstr.hpp"
#include "hades/row.hpp"

namespace hades
{
    class basic_filter
    {
        public:
            virtual void bind(sqlite3_stmt *stmt) const = 0;
            virtual std::string clause() const = 0;
    };

    template<typename ...Values>
    class where : public basic_filter
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
            void bind(sqlite3_stmt *stmt) const override
            {
                bind_values(m_values, stmt);
            }
            std::string clause() const override
            {
                return mkstr() << " WHERE " << m_clause;
            }
        private:
            const std::string m_clause;
            hades::row<Values...> m_values;
    };

    class order_by : public basic_filter
    {
        public:
            order_by(const std::string& clause, const int limit=-1, const int offset=0) :
                m_clause(clause),
                m_limit(limit),
                m_offset(offset)
            {
            }
            std::string clause() const override
            {
                hades::mkstr out;
                out << "ORDER BY " << m_clause;
                if(m_limit != -1)
                    out << " LIMIT " << m_limit << " OFFSET " << m_offset;
                return out;
            }
            void bind(sqlite3_stmt *) const override
            {
            }
        private:
            const std::string m_clause;
            const int m_limit, m_offset;
    };

    template<typename Where>
    class filter : public basic_filter
    {
        public:
            filter(const Where& where_, const order_by& order_by_) :
                m_where(where_),
                m_order_by(order_by_)
            {
            }
            std::string clause() const override
            {
                return mkstr() << m_where.clause() << " " << m_order_by.clause();
            }
            void bind(sqlite3_stmt *stmt) const override
            {
                m_where.bind(stmt);
            }
        private:
            const Where m_where;
            const order_by m_order_by;
    };

    class filter_all : public basic_filter
    {
        public:
            std::string clause() const override
            {
                return "";
            }
            void bind(sqlite3_stmt *) const override
            {
            }
    };
}

#endif

