#ifndef HADES_FILTER_HPP
#define HADES_FILTER_HPP

#include <memory>

#include <boost/fusion/include/join.hpp>

#include "hades/bind_values.hpp"
#include "hades/mkstr.hpp"
#include "hades/row.hpp"

namespace hades
{
    class basic_filter
    {
        public:
            /*!
             * \brief Bind stored parameters to the SQLite statement starting
             * at the leftmost index (1).
             */
            virtual void bind(sqlite3_stmt *stmt) const = 0;
            /*!
             * \brief Retrieve the clause representing this filter.  It should
             * be added to the end of the SQL query.
             */
            virtual std::string clause() const = 0;
    };

    namespace detail
    {
        class basic_where : public basic_filter
        {
            public:
                virtual void bind(sqlite3_stmt *stmt) const = 0;
                virtual std::string clause() const = 0;
                virtual std::unique_ptr<basic_where> clone() const = 0;
        };
        /*!
         * \brief An SQL WHERE clause.  Should be instantiated via an overload
         * of hades::where rather than this template directly.
         */
        template<typename Row>
        class where : public basic_where
        {
            public:
                typedef Row values_type;
                /*!
                 * \brief A where clause which selects all tuples.
                 */
                where() :
                    m_clause("1"),
                    m_values(hades::row<>())
                {
                }
                where(const std::string& clause_, const Row& values) :
                    m_clause(clause_),
                    m_values(values)
                {
                }
                explicit where(const std::string& clause_) :
                    m_clause(clause_),
                    m_values(hades::row<>())
                {
                }
                where(const where<Row>& o) :
                    m_clause(o.m_clause),
                    m_values(o.m_values)
                {
                }
                std::unique_ptr<basic_where> clone() const override
                {
                    return std::unique_ptr<basic_where>(
                            new detail::where<Row>(*this)
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
                std::string basic_clause() const
                {
                    return m_clause;
                }
                Row values() const
                {
                    return m_values;
                }
            private:
                const std::string m_clause;
                Row m_values;
        };

        template<typename X, typename Y>
        class and_ : public basic_where
        {
            public:
                and_(const X& x, const Y& y) :
                    m_x(x),
                    m_y(y)
                {
                }
                std::string clause() const override
                {
                    return mkstr() << "WHERE (" << m_x.basic_clause() <<
                        ") AND (" << m_y.basic_clause() << ")";
                }
                void bind(sqlite3_stmt *stmt) const override
                {
                    bind_values(
                            boost::fusion::join(m_x.values(), m_y.values()),
                            stmt
                            );
                }
                std::unique_ptr<basic_where> clone() const override
                {
                    return std::unique_ptr<and_<X, Y>>(new and_(m_x, m_y));
                }
            private:
                X m_x;
                Y m_y;
        };
    }

    /*!
     * \brief WHERE filter that accepts all tuples.
     */
    detail::where<hades::row<>> where();
    /*!
     * \brief WHERE filter with no substitution parameters.
     */
    detail::where<hades::row<>> where(const std::string& clause_);
    /*!
     * \brief WHERE filter with substitution parameters.  The parameters will
     * be substituted by SQLite when the clause has been fully constructed.
     */
    template<typename Row>
    detail::where<Row> where(const std::string& clause_, const Row& row)
    {
        return detail::where<Row>(clause_, row);
    }

    /*!
     * \brief Build a WHERE clause that is a conjunction of two given WHERE
     * clauses.
     *
     * \param X a hades::detail::where.
     * \param Y a hades::detail::where.
     */
    template<typename X, typename Y>
    typename detail::and_<X, Y> and_(const X& x, const Y& y)
    {
        return detail::and_<X, Y>(x, y);
    }

    /*!
     * \brief A filter comprising ORDER BY, LIMIT and OFFSET clauses.
     */
    class order_by : public basic_filter
    {
        public:
            /*!
             * \brief Create an ORDER_BY filter with optional limit and offset.
             *
             * Either offset or both limit and offset may be omitted.
             */
            order_by(const std::string& clause, const int limit=-1, const int offset=0) :
                m_clause(clause),
                m_limit(limit),
                m_offset(offset)
            {
            }
            order_by(const order_by& o) :
                m_clause(o.m_clause),
                m_limit(o.m_limit),
                m_offset(o.m_offset)
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
            std::string m_clause;
            int m_limit, m_offset;
    };

    /*!
     * \brief Filter comprising a WHERE clause and an ORDER BY/LIMIT/OFFSET
     * clause.
     */
    class filter : public basic_filter
    {
        public:
            filter(
                    const detail::basic_where& where_,
                    const order_by& order_by_
                    ) :
                m_where(where_.clone()),
                m_order_by(order_by_)
            {
            }
            filter(filter&& o) :
                m_order_by(std::move(o.m_order_by))
            {
                m_where = std::move(o.m_where);
            }
            std::string clause() const override
            {
                return mkstr() << m_where->clause() << " " << m_order_by.clause();
            }
            void bind(sqlite3_stmt *stmt) const override
            {
                m_where->bind(stmt);
            }
        private:
            std::unique_ptr<detail::basic_where> m_where;
            const order_by m_order_by;
    };

    /*!
     * \brief Filter accepting all tuples.
     */
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
