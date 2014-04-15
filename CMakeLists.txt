cmake_minimum_required(VERSION 2.8)
project(galaxpy)

set(CMAKE_CXX_FLAGS "-fPIC ${CMAKE_CXX_FLAGS}")
add_subdirectory(lib/jupiter)
add_subdirectory(lib/libsaturn)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(OUTPUT ${CMAKE_BINARY_DIR})
message(STATUS ${OUTPUT})

find_program(PYTHON3 python3)

EXECUTE_PROCESS(
    COMMAND ${PYTHON3} -c "
import sys
q=sys.version_info;
sys.stdout.write('{}.{}'.format(q.major,q.minor))
    "
    OUTPUT_VARIABLE PYTHON_VERSION
)

if (WIN32)
    set(PLATFORM "windows")
    message(FATAL_ERROR "Untested on windows")
else()
    set(PLATFORM "linux")
    EXECUTE_PROCESS(
        COMMAND uname -m
        COMMAND tr -d '\n'
        OUTPUT_VARIABLE ARCHITECTURE
    )
endif()

set(PREFIX "${PLATFORM}-${ARCHITECTURE}-${PYTHON_VERSION}")
message(STATUS "Prefix: ${PREFIX}")
# temp.linux-x86_64-3.3/

add_custom_command(
    OUTPUT ${OUTPUT}/timestamp ${CMAKE_CURRENT_BINARY_DIR}/lib.${PREFIX}
    COMMAND ${PYTHON3} ${CMAKE_CURRENT_SOURCE_DIR}/setup.py build
    COMMAND ${CMAKE_COMMAND} -E touch ${OUTPUT}/timestamp
    DEPENDS libjupiter libsaturn
)

add_custom_target(target ALL DEPENDS ${OUTPUT}/timestamp)

