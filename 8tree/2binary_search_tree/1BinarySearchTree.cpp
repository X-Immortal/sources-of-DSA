//
// Created by xyx on 2025/10/11.
//
#include <iostream>
#include <stdexcept>

using std::cout;
using std::endl;

template<class K>
concept Comparable = requires(K &a, K &b)
{
    { a < b } -> std::convertible_to<bool>;
};

template<Comparable K, class V>
class BinarySearchTree {
    struct SBSTNode {
        // key是唯一的
        K key;
        V value;
        SBSTNode *left;
        SBSTNode *right;

        explicit SBSTNode(K key, V &&value, SBSTNode *left = nullptr, SBSTNode *right = nullptr) :
        key(key), value(std::forward<V>(value)), left(left), right(right) {}
    };

    SBSTNode *root;

public:
    BinarySearchTree() {

    }

    V get(K key) {
        SBSTNode *cur = root;
        while (cur != nullptr) {
            if (key < cur->key) {
                cur = cur->left;
            } else if (cur->key < key) {
                cur = cur->right;
            } else {
                return cur->value;
            }
        }
        throw std::runtime_error("key not found!");
    }

    V min() {
        if (root == nullptr) {
            throw std::runtime_error("tree is empty!");
        }
        SBSTNode *cur = root;
        while (cur->left != nullptr) {
            cur = cur->left;
        }
        return cur->value;
    }

    V max() {
        if (root == nullptr) {
            throw std::runtime_error("tree is empty!");
        }
        SBSTNode *cur = root;
        while (cur->right != nullptr) {
            cur = cur->right;
        }
        return cur->value;
    }

    void put(int key, V &&value) {

    }

    V successor(int key) {

    }

    V predecessor(int key) {

    }

    V del(int key) {

    }
};

int main() {
    return 0;
}