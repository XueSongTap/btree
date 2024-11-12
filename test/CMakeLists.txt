# 测试目录的CMakeLists.txt
set(CMAKE_BUILD_TYPE Debug)
set(TEST_SOURCES
    btree_test.cpp
    btree_performance_test.cpp  # 性能测试
    btree_stress_test.cpp       # 压力测试
)

# 创建测试可执行文件
add_executable(btree_test ${TEST_SOURCES})

# 链接必要的库
target_link_libraries(btree_test
    PRIVATE
    btree                  # 主项目库
    GTest::gtest          # Google Test主库
    GTest::gtest_main     # Google Test主函数
    GTest::gmock          # Google Mock (如果需要)
)

# 包含目录
target_include_directories(btree_test
    PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

# 设置测试属性
set_target_properties(btree_test
    PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

# 添加测试到CTest
gtest_discover_tests(btree_test
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    PROPERTIES
    TIMEOUT 120  # 设置测试超时时间（秒）
)

# 可选：添加性能测试目标
if(BUILD_PERFORMANCE_TESTS)
    add_executable(btree_perf_test btree_performance_test.cpp)
    target_link_libraries(btree_perf_test
        PRIVATE
        btree
        benchmark::benchmark  # Google Benchmark库
    )
endif()

# 可选：设置测试覆盖率
if(ENABLE_COVERAGE)
    target_compile_options(btree_test PRIVATE --coverage)
    target_link_libraries(btree_test PRIVATE gcov)
endif()

# 可选：添加自定义测试目标
add_custom_target(check
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
    DEPENDS btree_test
)