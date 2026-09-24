cmake_minimum_required(VERSION 3.21)

project(SmartPointers LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(
    CMAKE_RUNTIME_OUTPUT_DIRECTORY
    ${CMAKE_SOURCE_DIR}/bin
)

add_executable(smart_pointers
    main.cpp
)

target_include_directories(smart_pointers
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/lib
)

target_compile_options(smart_pointers
    PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)

add_subdirectory(
    third_party/googletest
    EXCLUDE_FROM_ALL
)

enable_testing()

add_subdirectory(tests)