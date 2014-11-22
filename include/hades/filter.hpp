#ifndef HADES_FILTER_HPP
#define HADES_FILTER_HPP

#include <memory>

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

    class basic_where_values
    {
        public:
            virtual void bind(sqlite3_stmt *stmt) const = 0;
            virtual std::unique_ptr<basic_where_values> clone() const = 0;
    };
    template<typename ...Values>
    class where_values :
        public basic_where_values
    {
        public:
            where_values(const hades::row<Values...> values) :
                m_values(values)
            {
            }
            void bind(sqlite3_stmt *stmt) const override
            {
                bind_values(m_values, stmt);
            }
            std::unique_ptr<basic_where_values> clone() const
            {
                return std::unique_ptr<basic_where_values>(
                        new where_values(m_values)
                        );
            }
        private:
            hades::row<Values...> m_values;
    };

    class where : public basic_filter
    {
        public:
            /*!
             * \brief A where clause which selects all tuples.
             */
            where() :
                m_clause("1"),
                m_values(new where_values<>(hades::row<>()))
            {
            }
            template<typename ...Values>
            where(const std::string& clause_, hades::row<Values...> values) :
                m_clause(clause_),
                m_values(new where_values<Values...>(values))
            {
            }
            where(const std::string& clause_) :
                m_clause(clause_),
                m_values(new where_values<>(hades::row<>()))
            {
            }
            where(const where& o) :
                m_clause(o.m_clause),
                m_values(o.m_values->clone())
            {
            }
            void bind(sqlite3_stmt *stmt) const override
            {
                m_values->bind(stmt);
            }
            std::string clause() const override
            {
                return mkstr() << " WHERE " << m_clause;
            }
        private:
            const std::string m_clause;
            std::unique_ptr<basic_where_values> m_values;
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
            //order_by(const order_by& o) :
                //m_clause(o.m_clause),
                //m_limit(o.m_limit),
                //m_offset(o.m_offset)
            //{
            //}
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

    class filter : public basic_filter
    {
        public:
            filter(const where& where_, const order_by& order_by_) :
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
            const where m_where;
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

