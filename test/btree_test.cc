#include <gtest/gtest.h>
#include "../include/btree.h"
#include <algorithm>
#include <random>
class BTreeTest : public ::testing::Test {
protected:
    BTree<int> btree{3}; // 度数为3的B树

    void SetUp() override {
        // 在每个测试用例前执行
    }

    void TearDown() override {
        // 在每个测试用例后执行
    }
};


TEST_F(BTreeTest, EmptyTreeTest) {
    // Test searching in an empty tree
    auto result = btree.search(10);
    EXPECT_FALSE(result.has_value());

    // Test deleting from an empty tree
    EXPECT_FALSE(btree.remove(10));
}

TEST_F(BTreeTest, SequentialInsertionTest) {
    // Insert sequential keys and verify
    for (int i = 1; i <= 20; ++i) {
        btree.insert(i);
    }
    for (int i = 1; i <= 20; ++i) {
        auto result = btree.search(i);
        EXPECT_TRUE(result.has_value());
        EXPECT_EQ(result.value(), i);
    }
}

TEST_F(BTreeTest, RandomInsertionDeletionTest) {
    std::vector<int> keys(100);
    std::iota(keys.begin(), keys.end(), 1);
    std::random_device rd;
    std::mt19937 g(rd());

    // Randomly shuffle and insert keys
    std::shuffle(keys.begin(), keys.end(), g);
    for (int key : keys) {
        btree.insert(key);
    }

    // Verify all keys are present
    for (int i = 1; i <= 100; ++i) {
        auto result = btree.search(i);
        EXPECT_TRUE(result.has_value());
    }

    // Randomly shuffle and delete keys
    std::shuffle(keys.begin(), keys.end(), g);
    for (int key : keys) {
        EXPECT_TRUE(btree.remove(key));
    }

    // Verify all keys are deleted
    for (int i = 1; i <= 100; ++i) {
        auto result = btree.search(i);
        EXPECT_FALSE(result.has_value());
    }
}


TEST_F(BTreeTest, DeletionFromLeafTest) {
    btree.insert(10);
    btree.insert(20);
    btree.insert(5);
    EXPECT_TRUE(btree.remove(5));
    EXPECT_FALSE(btree.search(5).has_value());
}

TEST_F(BTreeTest, InsertionTest) {
    btree.insert(10);
    btree.insert(20);
    btree.insert(5);
    
    auto result = btree.search(10);
    EXPECT_TRUE(result.has_value());
    EXPECT_EQ(result.value(), 10);
    
    result = btree.search(15);
    EXPECT_FALSE(result.has_value());
}

TEST_F(BTreeTest, DeletionTest) {
    btree.insert(10);
    btree.insert(20);
    btree.insert(5);
    
    EXPECT_TRUE(btree.remove(10));
    EXPECT_FALSE(btree.search(10).has_value());
    EXPECT_FALSE(btree.remove(15)); // 删除不存在的键
}

TEST_F(BTreeTest, SplitTest) {
    // 每插入一个数就验证树的状态
    for (int i = 0; i < 10; i++) {
        btree.insert(i);
        auto root = btree.get_root();
        ASSERT_TRUE(root != nullptr);
        EXPECT_LE(root->keys.size(), 2 * btree.get_min_degree() - 1);
        
        // 验证插入的数是否存在
        auto result = btree.search(i);
        EXPECT_TRUE(result.has_value());
        EXPECT_EQ(result.value(), i);
    }
}
TEST_F(BTreeTest, StressTest) {
    std::vector<int> numbers;
    for (int i = 0; i < 1000; i++) {
        numbers.push_back(i);
    }
    
    // 随机打乱并插入
    std::random_shuffle(numbers.begin(), numbers.end());
    for (int num : numbers) {
        btree.insert(num);
    }
    
    // 验证所有数字都能找到
    for (int i = 0; i < 1000; i++) {
        auto result = btree.search(i);
        EXPECT_TRUE(result.has_value());
        EXPECT_EQ(result.value(), i);
    }
}