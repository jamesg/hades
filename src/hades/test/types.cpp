#include "types.hpp"

#include "hades/devoid.hpp"

const char hades::test::attr::site_id[] = "site_id";
const char hades::test::attr::device_id[] = "device_id";
const char hades::test::attr::name[] = "name";
const char hades::test::attr::city[] = "city";
const char hades::test::attr::batch_id[] = "batch_id";
const char hades::test::attr::variety_id[] = "variety_id";
const char hades::test::attr::family_id[] = "family_id";
const char hades::test::flag::site::enabled[] = "site_enabled";
const char hades::test::relvar::site[] = "site";
const char hades::test::relvar::device[] = "device";
const char hades::test::relvar::batch[] = "batch";
const char hades::test::relvar::variety[] = "variety";
const char hades::test::relvar::family[] = "family";

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
            "CREATE TABLE site_enabled ( "
            " site_id INTEGER PRIMARY KEY, "
            " FOREIGN KEY(site_id) REFERENCES site(site_id) "
            " )",
            conn
            );
    hades::devoid(
            "CREATE TABLE site_location ( "
            " site_id INTEGER PRIMARY KEY, "
            " city VARCHAR, "
            " FOREIGN KEY(site_id) REFERENCES site(site_id) "
            " )",
            conn
            );
    hades::devoid(
            "CREATE TABLE device ( "
            " site_id INTEGER, "
            " device_id INTEGER, "
            " name VARCHAR DEFAULT 'name_value', "
            " PRIMARY KEY (site_id, device_id), "
            " FOREIGN KEY(site_id) REFERENCES site(site_id) "
            " )",
            conn
            );
    hades::devoid(
            "CREATE TABLE device_enabled ( "
            " site_id INTEGER, "
            " device_id INTEGER, "
            " PRIMARY KEY (site_id, device_id), "
            " FOREIGN KEY(site_id) REFERENCES site(site_id) "
            " ) ",
            conn
            );
    hades::devoid(
            "CREATE TABLE family ( "
            " family_id INTEGER PRIMARY KEY "
            " ) ",
            conn
            );
    hades::devoid(
            "CREATE TABLE variety ( "
            " variety_id INTEGER PRIMARY KEY, "
            " family_id INTEGER, "
            " FOREIGN KEY(family_id) REFERENCES family(family_id) "
            " ) ",
            conn
            );
    hades::devoid(
            "CREATE TABLE batch ( "
            " batch_id INTEGER PRIMARY KEY, "
            " variety_id INTEGER, "
            " FOREIGN KEY(variety_id) REFERENCES variety(variety_id) "
            " ) ",
            conn
            );
}

