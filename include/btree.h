#pragma once
#include <vector>
#include <memory>
#include <optional>
#include <algorithm>

template <typename T>
class BTree {
private:
    struct Node {
        std::vector<T> keys;
        std::vector<std::shared_ptr<Node>> children;
        bool leaf;
        
        Node(bool leaf = true) : leaf(leaf) {}

        bool is_full(int t) const {
            return keys.size() == 2 * t - 1;
        }

        bool is_minimum(int t) const {
            return keys.size() == t - 1;
        }
    };
    
    std::shared_ptr<Node> root;
    int t;  // minimum degree

    void split_child(std::shared_ptr<Node>& parent, int index) {
        if (!parent) {
            throw std::runtime_error("Null parent in split_child");
        }

        // Make a copy of the child
        auto child = parent->children[index];
        if (!child) {
            throw std::runtime_error("Null child in split_child");
        }

        auto new_node = std::make_shared<Node>(child->leaf);

        new_node->keys.reserve(t - 1);
        if (!child->leaf) {
            new_node->children.reserve(t);
        }

        // Copy the last (t - 1) keys of child to new_node
        for (int j = 0; j < t - 1; j++) {
            new_node->keys.push_back(child->keys[j + t]);
        }

        // If child is not leaf, copy its last t children to new_node
        if (!child->leaf) {
            for (int j = 0; j < t; j++) {
                new_node->children.push_back(child->children[j + t]);
            }
        }

        // Reduce the number of keys in child
        child->keys.resize(t - 1);
        if (!child->leaf) {
            child->children.resize(t);
        }

        // Insert new key and child into parent
        parent->keys.insert(parent->keys.begin() + index, child->keys[t - 1]);
        parent->children.insert(parent->children.begin() + index + 1, new_node);
    }

    void insert_non_full(std::shared_ptr<Node>& node, const T& key) {
        if (!node) {
            throw std::runtime_error("Null node in insert_non_full");
        }

        int i = static_cast<int>(node->keys.size()) - 1;

        if (node->leaf) {
            node->keys.reserve(node->keys.size() + 1);
            while (i >= 0 && key < node->keys[i]) {
                i--;
            }
            node->keys.insert(node->keys.begin() + i + 1, key);
        } else {
            while (i >= 0 && key < node->keys[i]) {
                i--;
            }
            i++;

            if (i >= node->children.size()) {
                throw std::runtime_error("Invalid child index in insert_non_full");
            }

            if (node->children[i]->keys.size() == 2 * t - 1) {
                split_child(node, i);
                if (key > node->keys[i]) {
                    i++;
                }
            }
            insert_non_full(node->children[i], key);
        }
    }
    std::optional<T> search_internal(const std::shared_ptr<Node>& node, const T& key) const {
        if (!node) {
            return std::nullopt;
        }

        int i = 0;
        while (i < node->keys.size() && key > node->keys[i]) {
            i++;
        }

        if (i < node->keys.size() && key == node->keys[i]) {
            return node->keys[i];
        }

        if (node->leaf) {
            return std::nullopt;
        }

        return search_internal(node->children[i], key);
    }

    bool remove_internal(std::shared_ptr<Node>& node, const T& key) {
        if (!node) {
            return false;
        }

        int i = 0;
        while (i < node->keys.size() && key > node->keys[i]) {
            i++;
        }

        if (i < node->keys.size() && key == node->keys[i]) {
            if (node->leaf) {
                node->keys.erase(node->keys.begin() + i);
                return true;
            }
        }

        if (node->leaf) {
            return false;
        }

        return remove_internal(node->children[i], key);
    }

public:
    BTree(int min_degree) : t(min_degree) {
        if (min_degree < 2) {
            throw std::invalid_argument("Minimum degree must be at least 2");
        }
        root = std::make_shared<Node>();
    }

    const std::shared_ptr<Node>& get_root() const { return root; }
    int get_min_degree() const { return t; }
    int get_max_keys() const { return 2 * t - 1; }
    int get_min_keys() const { return t - 1; }

    void insert(const T& key) {
        if (root->keys.size() == 2 * t - 1) {
            auto new_root = std::make_shared<Node>(false);
            new_root->children.push_back(root);
            root = new_root;
            split_child(root, 0); // Updated call without child parameter
            insert_non_full(root, key);
        } else {
            insert_non_full(root, key);
        }
    }

    std::optional<T> search(const T& key) const {
        return search_internal(root, key);
    }
    
    bool remove(const T& key) {
        return remove_internal(root, key);
    }
};