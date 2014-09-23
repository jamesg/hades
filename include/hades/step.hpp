#ifndef HADES_STEP
#define HADES_STEP

#include <sqlite3.h>

namespace hades
{
    class connection;

    /*!
     * \brief Call sqlite3_step, converting non-success return codes into
     * exceptions.
     */
    void step(sqlite3_stmt *stmt);
    /*!
     * \brief Call sqlite3_step, converting non-success return codes into
     * exceptions.
     * \note The supplied connection object will be used to improve error
     * reporting.
     */
    void step(sqlite3_stmt *stmt, connection&);
}

#endif

