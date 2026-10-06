cmake_minimum_required(VERSION 3.14)
project(windows-cinter)

set(CMAKE_CXX_STANDARD 20)

file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

# add_link_options("/PROFILE")

# add_compile_definitions(UNICODE)
# add_compile_definitions(WIN32_LEAN_AND_MEAN)
# add_compile_definitions(NOMINMAX)

add_library(wci_core STATIC
    source/core/console.cpp
    source/core/eventloop.cpp
    source/core/frameloop.cpp
    source/core/window.cpp
    source/ui/menulist.cpp
)

add_executable(snake
    snake/snake.cpp
    source/core/console.cpp
    source/core/frameloop.cpp
    source/core/window.cpp
    source/ui/menulist.cpp
)

add_executable(console
    examples/console.cpp
    source/core/console.cpp
)

add_executable(events
    examples/events.cpp
    source/core/console.cpp
    source/core/eventloop.cpp
    source/core/window.cpp
)

add_executable(frames
    examples/frames.cpp
    source/core/console.cpp
    source/core/frameloop.cpp
    source/core/window.cpp
)

add_executable(matrix
    examples/matrix.cpp
    source/core/console.cpp
    source/core/window.cpp
)

add_executable(moving_panels
    examples/moving_panels.cpp
    source/core/console.cpp
    source/core/eventloop.cpp
    source/core/window.cpp
)

add_executable(window
    examples/window.cpp
    source/core/console.cpp
    source/core/window.cpp
)
