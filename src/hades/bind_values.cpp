#include "hades/bind_values.hpp"

#ifdef HADES_ENABLE_DEBUGGING
#include <iostream>
#endif

#include "hades/exception.hpp"
#include "hades/mkstr.hpp"

void hades::bind_any(int index, const styx::element& element, sqlite3_stmt *stmt)
{
    struct bind_visitor : public boost::static_visitor<void>
    {
        int index;
        sqlite3_stmt *stmt;

        bind_visitor(int index_, sqlite3_stmt *stmt_) :
            index(index_),
            stmt(stmt_)
        {
        }

        void operator()(const std::string& s) const
        {
            hades::bind(index, s, stmt);
        }

        void operator()(const int& i) const
        {
            hades::bind(index, i, stmt);
        }

        void operator()(const double& d) const
        {
            hades::bind(index, d, stmt);
        }

        void operator()(const bool& b) const
        {
            hades::bind(index, static_cast<int>(b), stmt);
        }

        void operator()(const styx::list& o) const
        {
            throw hades::exception("binding list to SQL statement");
        }

        void operator()(const styx::object& o) const
        {
            throw hades::exception("binding object to SQL statement");
        }

        void operator()(const styx::null_t&) const
        {
#ifdef HADES_ENABLED_DEBUGGING
            std::cerr << "binding null" << std::endl;
#endif
            hades::bind_null(index, stmt);
        }
    };

    boost::apply_visitor(bind_visitor(index, stmt), element);
}

void hades::bind(int index, const std::string& s, sqlite3_stmt *stmt)
{
    if(sqlite3_bind_text(stmt, index, s.c_str(), s.length(), nullptr) != SQLITE_OK)
        throw hades::exception(
            mkstr() << "binding string \"" << s << "\" to index " << index
            );
}
void hades::bind(int index, const double value, sqlite3_stmt *stmt)
{
    if(sqlite3_bind_double(stmt, index, value) != SQLITE_OK)
        throw hades::exception(
            mkstr() << "binding double " << value << " to index " << index
            );
}
void hades::bind(int index, const int value, sqlite3_stmt *stmt)
{
    if(sqlite3_bind_int(stmt, index, value) != SQLITE_OK)
        throw hades::exception(
            mkstr() << "binding int " << value << " to index " << index
            );
}

void hades::bind(int index, const void* value, int n, sqlite3_stmt *stmt)
{
    if(sqlite3_bind_blob(stmt, index, value, n, nullptr) != SQLITE_OK)
        throw hades::exception(
            mkstr() << "binding BLOB of length " << n << " to index " << index
            );
}

void hades::bind_null(int index, sqlite3_stmt *stmt)
{
    if(sqlite3_bind_null(stmt, index) != SQLITE_OK)
        throw hades::exception(mkstr() << "binding null to index " << index);
}

