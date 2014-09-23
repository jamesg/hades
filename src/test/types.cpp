#include "types.hpp"

#include "hades/devoid.hpp"

const char hades::test::attr::site_id[] = "site_id";
const char hades::test::attr::device_id[] = "device_id";
const char hades::test::attr::name[] = "name";
const char hades::test::attr::city[] = "city";
const char hades::test::relvar::site[] = "site";
const char hades::test::relvar::device[] = "device";

void hades::test::create(hades::connection& conn)
{
    hades::devoid(
            "CREATE TABLE site ( "
            " site_id INTEGER PRIMARY KEY AUTOINCREMENT, "
            " name VARCHAR DEFAULT 'name_value' "
            " )",
            conn
            );
    hades::devoid(
            "CREATE TABLE site_location ( "
            " site_id INTEGER PRIMARY KEY, "
            " city VARCHAR "
            " )",
            conn
            );
    hades::devoid(
            "CREATE TABLE device ( "
            " site_id INTEGER, "
            " device_id INTEGER, "
            " name VARCHAR DEFAULT 'name_value', "
            " PRIMARY KEY (site_id, device_id) "
            " )",
            conn
            );
}

