add_library(
    sqlite3
    "src/sqlite3.c"
    )

target_link_libraries(
    sqlite3
    "dl"
    "pthread"
    )

