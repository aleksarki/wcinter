cmake_minimum_required(VERSION 3.14)
project(cinter)

set(CMAKE_CXX_STANDARD 20)

file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

add_executable(main
    main.cpp
    source/console.cpp
    source/window.cpp
)
