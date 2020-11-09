cmake_minimum_required(VERSION 2.8)
project (echeck C)

find_package(Intl)
add_executable(echeck echeck.c unicode.c)
add_executable(tests echeck.c unicode.c tests.c CuTest.c)
target_compile_definitions (tests PRIVATE TESTING=1)
target_link_libraries(echeck ${Intl_LIBRARIES})
target_link_libraries(tests ${Intl_LIBRARIES})
include_directories(${Intl_INCLUDE_DIR})

