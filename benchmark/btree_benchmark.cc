#include <benchmark/benchmark.h>
#include "../include/btree.h"
#include <random>

static void BM_BTreeInsertion(benchmark::State& state) {
    BTree<int> btree(50); // B-tree with large degree
    std::mt19937 rng;
    std::uniform_int_distribution<int> dist(1, 1000000);

    for (auto _ : state) {
        // Benchmark insertion time
        int key = dist(rng);
        btree.insert(key);
    }
}

BENCHMARK(BM_BTreeInsertion);

static void BM_BTreeSearch(benchmark::State& state) {
    BTree<int> btree(50);
    std::vector<int> keys(state.range(0));
    std::iota(keys.begin(), keys.end(), 0);

    // Insert keys
    for (int key : keys) {
        btree.insert(key);
    }

    std::mt19937 rng;
    std::uniform_int_distribution<int> dist(0, keys.size() - 1);

    for (auto _ : state) {
        // Benchmark search time
        int key = dist(rng);
        benchmark::DoNotOptimize(btree.search(key));
    }
}

BENCHMARK(BM_BTreeSearch)->Range(1<<10, 1<<20);

static void BM_BTreeDeletion(benchmark::State& state) {
    BTree<int> btree(50);
    std::vector<int> keys(state.range(0));
    std::iota(keys.begin(), keys.end(), 0);

    // Insert keys
    for (int key : keys) {
        btree.insert(key);
    }

    // Random shuffle keys for deletion
    std::shuffle(keys.begin(), keys.end(), std::mt19937{std::random_device{}()});

    auto it = keys.begin();

    for (auto _ : state) {
        if (it == keys.end()) {
            // Re-initialize B-Tree and iterator
            btree = BTree<int>(50);
            for (int key : keys) {
                btree.insert(key);
            }
            std::shuffle(keys.begin(), keys.end(), std::mt19937{std::random_device{}()});
            it = keys.begin();
        }
        // Benchmark deletion time
        btree.remove(*it);
        ++it;
    }
}

BENCHMARK(BM_BTreeDeletion)->Range(1<<10, 1<<20);

BENCHMARK_MAIN();