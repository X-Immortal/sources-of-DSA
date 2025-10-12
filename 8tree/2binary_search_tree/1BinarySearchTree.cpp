//
// Created by xyx on 2025/10/11.
//
#include <iostream>
#include <string>
#include <functional>

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

        // 万能引用+完美转发
        explicit SBSTNode(K &&key, V &&value, SBSTNode *left = nullptr, SBSTNode *right = nullptr) :
        key(std::forward<K>(key)), value(std::forward<V>(value)), left(left), right(right) {}
    };

    SBSTNode *root;

public:
    BinarySearchTree() : root(nullptr) {}

    ~BinarySearchTree() {
        post_order(root, [](SBSTNode *node) {
           delete node;
        });
    }

    V get(const K &key) {
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

    void put(K &&key, V &&value) {
        if (root == nullptr) {
            root = new SBSTNode(std::forward<K>(key), std::forward<V>(value));
            return;
        }

        SBSTNode *cur = root;
        SBSTNode *parent = nullptr;
        while (cur != nullptr) {
            parent = cur;
            if (key < cur->key) {
                cur = cur->left;
            } else if (cur->key < key) {
                cur = cur->right;
            } else {
                cur->value = std::forward<V>(value);
                return;
            }
        }

        if (key < parent->key) {
            parent->left = new SBSTNode(std::forward<K>(key), std::forward<V>(value));
        } else {
            parent->right = new SBSTNode(std::forward<K>(key), std::forward<V>(value));
        }
    }

    V successor(int key) {

    }

    V predecessor(int key) {

    }

    V del(int key) {

    }

    void print() {
        post_order(root, [](SBSTNode *node) {
            cout << node->key << ": " << node->value << endl;
        });
    }

private:
    void post_order(SBSTNode *node, const std::function<void(SBSTNode *)> &func) {
        if (node == nullptr) {
            return;
        }
        func(node);
        post_order(node->left, func);
        post_order(node->right, func);
    }
};

int main() {
    BinarySearchTree<int, std::string> tree;
    tree.put(4, "4");
    tree.put(2, "2");
    tree.put(6, "6");
    tree.put(1, "1");
    tree.put(3, "3");
    tree.put(7, "7");
    tree.put(5, "5");
    tree.print();
    tree.put(1, "0");
    cout << tree.get(1) << endl;
    cout << tree.min() << endl;
    cout << tree.max() << endl;
    return 0;
}
