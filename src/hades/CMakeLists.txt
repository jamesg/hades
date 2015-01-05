file(GLOB hades_sources *.cpp detail/*.cpp)
file(GLOB hades_test_sources test/*.cpp)

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
