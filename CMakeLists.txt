cmake_minimum_required(VERSION 2.8)
project (echeck C)

add_executable(echeck echeck.c unicode.c whereami.c)
add_executable(tests echeck.c unicode.c tests.c whereami.c CuTest.c)
target_compile_definitions (tests PRIVATE TESTING=1)

if (WIN32)
find_library(Intl_LIBRARY NAMES libintl intl
  DOC "libintl libraries (if not in the C library)")
endif (WIN32)
if (CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
target_compile_definitions (echeck PRIVATE -D_DEFAULT_SOURCE)
endif()
find_package(Intl)
include_directories(${Intl_INCLUDE_DIR})

if (Intl_FOUND)
target_compile_definitions (echeck PRIVATE HAVE_GETTEXT=1)
target_compile_definitions (tests PRIVATE HAVE_GETTEXT=1)
target_link_libraries(echeck ${Intl_LIBRARIES})
target_link_libraries(tests ${Intl_LIBRARIES})
else (Intl_FOUND)
message(STATUS "libintl not found")
endif (Intl_FOUND)

include("Localization.cmake")
SET (PACKAGE_NAME echeck)
PROGRAM_LOCALIZATION()
