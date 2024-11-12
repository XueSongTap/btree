cmake_minimum_required(VERSION 3.10)
project(btree_project)
set(CMAKE_BUILD_TYPE Debug)
# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 主库
add_library(btree INTERFACE)
target_include_directories(btree INTERFACE include)

# Google Test
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.12.1
)
FetchContent_MakeAvailable(googletest)

# 测试
enable_testing()
add_executable(btree_test test/btree_test.cc)
target_link_libraries(btree_test
    PRIVATE
    btree
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(btree_test)

include(FetchContent)
FetchContent_Declare(
    benchmark
    GIT_REPOSITORY https://github.com/google/benchmark.git
    GIT_TAG v1.7.1 # Use the latest stable release
)
FetchContent_MakeAvailable(benchmark)

# Link benchmark to your executable
add_executable(btree_benchmark benchmark/btree_benchmark.cc)
target_link_libraries(btree_benchmark PRIVATE benchmark::benchmark)