add_library(pointer_benchmark
    Benchmark.cpp
    Benchmark.hpp
)

target_include_directories(pointer_benchmark
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}

    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../lib
)

target_compile_options(pointer_benchmark PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)
