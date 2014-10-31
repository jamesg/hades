cmake_minimum_required(VERSION 2.6)

project(Hades)
enable_testing()

include_directories("include")

file(GLOB hades_sources src/*.cpp src/detail/*.cpp)
file(GLOB hades_test_sources src/test/*.cpp)

add_library(
    hades
    ${hades_sources}
    )

target_link_libraries(
    hades
    sqlite3
    )

add_executable(
    hades-test
    ${hades_test_sources}
    )
add_test(hades-test hades-test)

target_link_libraries(
    hades-test
    hades
    styx
    )

