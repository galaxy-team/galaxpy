cmake_minimum_required(VERSION 2.8)
project(galaxpy)

# if a library is going to be linked with a python extension,
# it needs to be compiled with the -fPIC flag
set(CMAKE_CXX_FLAGS "-fPIC ${CMAKE_CXX_FLAGS}")
add_subdirectory(lib/jupiter)
add_subdirectory(lib/libsaturn)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(OUTPUT ${CMAKE_BINARY_DIR})
message(STATUS ${OUTPUT})

find_program(PYTHON3 python3)
if (PYDEBUG)
    set(PYTHON3 ${PYTHON3}-dbg)
endif()

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

set(POSTFIX "${PLATFORM}-${ARCHITECTURE}-${PYTHON_VERSION}")

if (PYDEBUG)
    set(PYDEBUG_POSTFIX "-pydebug")
endif()


FILE(GLOB DEPENDENCIES RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*")

set(DEPENDENCIES
    ${CMAKE_CURRENT_SOURCE_DIR}/setup.py
    ${FILES}
)




set(OUTPUT_FILES
    ${CMAKE_CURRENT_BINARY_DIR}/lib.${POSTFIX}${PYDEBUG_POSTFIX}
    ${CMAKE_CURRENT_BINARY_DIR}/temp.${POSTFIX}${PYDEBUG_POSTFIX}
    ${CMAKE_CURRENT_BINARY_DIR}/lib.${POSTFIX}${PYDEBUG_POSTFIX}/__init__.py
)

add_custom_command(
    OUTPUT ${OUTPUT_FILES}
    COMMAND ${CMAKE_COMMAND} -E chdir ${CMAKE_CURRENT_BINARY_DIR}/.. ${PYTHON3} ${CMAKE_CURRENT_SOURCE_DIR}/setup.py build -f
    COMMAND ${CMAKE_COMMAND} -E touch ${CMAKE_CURRENT_BINARY_DIR}/lib.${POSTFIX}/__init__.py
    DEPENDS libjupiter libsaturn libpluto ${DEPENDENCIES}
)

add_custom_target(python_library ALL DEPENDS ${OUTPUT_FILES})
