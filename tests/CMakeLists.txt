add_executable(
    run_tests_smart_pointers
    SharedPtrTests.cpp
    UniquePtrTests.cpp
)

target_include_directories(run_tests_smart_pointers PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../lib
)

target_compile_options(run_tests_smart_pointers PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)

target_link_libraries(run_tests_smart_pointers PRIVATE
    gtest_main
)

include(GoogleTest)

gtest_discover_tests(run_tests_smart_pointers)