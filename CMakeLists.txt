cmake_minimum_required(VERSION 2.8)
project (echeck C)

add_executable(echeck echeck.c unicode.c)
add_executable(tests echeck.c unicode.c tests.c CuTest.c)
target_compile_definitions (tests PRIVATE TESTING=1)
