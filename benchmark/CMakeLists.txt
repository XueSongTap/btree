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