cmake_minimum_required(VERSION 2.6)

project(Hades)
enable_testing()

include_directories("include")
include_directories("lib/catch/include")
include_directories("lib/jsmn/include")
include_directories("lib/sqlite3/include")

add_subdirectory(lib)
add_subdirectory(src)

find_package(Doxygen)
if(DOXYGEN_FOUND)
    configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/doxygen/doxygen.conf.in
        ${CMAKE_CURRENT_BINARY_DIR}/doxygen/doxygen.conf @ONLY
        )
    add_custom_target(
        hades-doc
        ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/doxygen/doxygen.conf
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen" VERBATIM
        )
endif(DOXYGEN_FOUND)

