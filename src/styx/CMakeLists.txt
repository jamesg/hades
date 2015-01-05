file(GLOB styx_sources *.cpp)
file(GLOB styx_parsers_sources parsers/*.cpp)
file(GLOB styx_serialisers_sources serialisers/*.cpp)
file(GLOB styx_test_sources test/*.cpp)

add_library(
    styx
    ${styx_sources}
    ${styx_parsers_sources}
    ${styx_serialisers_sources}
    )

target_link_libraries(
    styx
    ${CMAKE_THREAD_LIBS_INIT}
    ${Boost_LIBRARIES}
    jsmn
    )

add_executable(styx-test ${styx_test_sources})
add_test(styx-test styx-test)

target_link_libraries(
    styx-test
    styx
    )

