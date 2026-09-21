cmake_minimum_required(VERSION 3.21)

project(SmartPointers LANGUAGES CXX)

add_executable(smart_pointers
    main.cpp
    lib/UniquePtr.hpp
    lib/SharedPtr.hpp
)

set_target_properties(smart_pointers PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

target_compile_options(smart_pointers PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)