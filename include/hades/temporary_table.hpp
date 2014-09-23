#ifndef HADES_TEMPORARY_TABLE_HPP
#define HADES_TEMPORARY_TABLE_HPP

#include "hades/devoid.hpp"
#include "hades/mkstr.hpp"
#include "hades/row.hpp"

namespace hades
{
    /*!
     * \brief
     */
    class temporary_table
    {
        public:
            temporary_table(
                    const std::string& name_,
                    const char *sql,
                    connection& conn
                    ) :
                m_name(name_),
                m_created(false),
                m_conn(conn)
            {
                devoid(
                    mkstr() << "CREATE TEMPORARY TABLE " << name_ << " " << sql,
                    empty_row(),
                    conn
                    );
                m_created = true;
            }
            ~temporary_table()
            {
                if(m_created)
                    devoid(
                        mkstr() << "DROP TABLE " << m_name,
                        empty_row(),
                        m_conn
                        );
            }
            const std::string& name() const
            {
                return m_name;
            }
        private:
            std::string m_name;
            bool m_created;
            connection& m_conn;
    };
}

#endif

