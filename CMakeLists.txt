cmake_minimum_required(VERSION 3.14)
project(windows-cinter)

set(CMAKE_CXX_STANDARD 20)

file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

# add_compile_definitions(UNICODE)
# add_compile_definitions(WIN32_LEAN_AND_MEAN)
# add_compile_definitions(NOMINMAX)

add_executable(console
    examples/console.cpp
    source/console.cpp
)

add_executable(events
    examples/events.cpp
    source/eventloop.cpp
    source/console.cpp
    source/window.cpp
)

add_executable(matrix
    examples/matrix.cpp
    source/console.cpp
    source/window.cpp
)

add_executable(moving_panels
    examples/moving_panels.cpp
    source/console.cpp
    source/eventloop.cpp
    source/window.cpp
)

add_executable(window
    examples/window.cpp
    source/console.cpp
    source/window.cpp
)
