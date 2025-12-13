//
// Created by xyx on 2025/12/10.
//
#include <iostream>
#include <functional>
#include <algorithm>

using std::cout;
using std::endl;

namespace AVLT {
    template<class K>
    concept Comparable = requires(const K &a, const K &b)
    {
        { a < b } -> std::convertible_to<bool>;
        { a == b } -> std::convertible_to<bool>;
    };

    template<Comparable K, class V>
    class AVLTree {
        struct AVLNode {
            K key;
            V value;
            AVLNode *left;
            AVLNode *right;
            int height;

            template<class TK, class TV>
                requires std::constructible_from<K, TK&&> && std::constructible_from<V, TV&&>
            AVLNode(TK &&key, TV &&value, AVLNode *left = nullptr, AVLNode *right = nullptr) : key(std::forward<TK>(key)),
                value(std::forward<TV>(value)), left(left), right(right), height(1) {
            }
        };

        AVLNode *root;

    public:
        AVLTree() : root(nullptr) {
        }

        ~AVLTree() {
            post_order(root, [](AVLNode *node) {
                delete node;
            });
        }

        template<class TK, class TV>
            requires std::constructible_from<K, TK&&> && std::constructible_from<V, TV&&>
        void put(TK &&key, TV &&value) {
            root = put(root, std::forward<TK>(key), std::forward<TV>(value));
        }

        V remove(const K &key) {
            AVLNode *node = nullptr;
            root = remove(root, key, node);
            if (node == nullptr) {
                throw std::runtime_error("key not found!");
            }

            V ret = node->value;
            delete node;
            return ret;
        }

        // 平均时间复杂度O(log(n))
        // 最坏时间复杂度O(log(n))，总是维持平衡
        V &get(const K &key) {
            AVLNode *cur = root;
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

        V &min() {
            return min(root)->value;
        }

        V &max() {
            return max(root)->value;
        }

        void print() {
            post_order(root, [](AVLNode *node) {
                cout << node->key << ": " << node->value << endl;
            });
        }

    private:
        void post_order(AVLNode *node, const std::function<void(AVLNode *)> &func) {
            if (node == nullptr) {
                return;
            }
            post_order(node->left, func);
            post_order(node->right, func);
            func(node);
        }

        AVLNode *max(AVLNode *node) {
            if (node == nullptr) {
                throw std::runtime_error("tree is empty!");
            }
            while (node->right != nullptr) {
                node = node->right;
            }
            return node;
        }

        AVLNode *min(AVLNode *node) {
            if (node == nullptr) {
                throw std::runtime_error("tree is empty!");
            }
            while (node->left != nullptr) {
                node = node->left;
            }
            return node;
        }

        int get_height(AVLNode *node) {
            if (node == nullptr) {
                return 0;
            }
            return node->height;
        }

        void update_height(AVLNode *node) {
            node->height = std::max(get_height(node->left), get_height(node->right)) + 1;
        }

        // 平衡因子
        int balance_factor(AVLNode *node) {
            return get_height(node->left) - get_height(node->right);
        }

        // 基础右旋
        // 时间复杂度：O(1)
        AVLNode *right_rotate(AVLNode *node) {
            AVLNode *root = node->left;
            node->left = root->right;
            root->right = node;
            update_height(node);
            update_height(root);
            return root;
        }

        // 基础左旋
        // 时间复杂度：O(1)
        AVLNode *left_rotate(AVLNode *node) {
            AVLNode *root = node->right;
            node->right = root->left;
            root->left = node;
            update_height(node);
            update_height(root);
            return root;
        }

        // 先左旋左子树，再右旋根节点
        AVLNode *left_right_rotate(AVLNode *node) {
            node->left = left_rotate(node->left);
            return right_rotate(node);
        }

        // 先右旋右子树，再左旋根节点
        AVLNode *right_left_rotate(AVLNode *node) {
            node->right = right_rotate(node->right);
            return left_rotate(node);
        }

        AVLNode *balance(AVLNode *node) {
            if (node == nullptr) {
                return nullptr;
            }
            int bf = balance_factor(node);
            if (bf > 1) {
                if (balance_factor(node->left) < 0) {
                    // LR
                    node = left_right_rotate(node);
                } else {
                    // LL
                    node = right_rotate(node);
                }
            } else if (bf < -1) {
                if (balance_factor(node->right) > 0) {
                    // RL
                    node = right_left_rotate(node);
                } else {
                    // RR
                    node = left_rotate(node);
                }
            }
            return node;
        }

        // 从插入节点开始，向上更新高度，并在第一个失衡节点进行旋转
        template<class TK, class TV>
            requires std::constructible_from<K, TK&&> && std::constructible_from<V, TV&&>
        AVLNode *put(AVLNode *node, TK &&key, TV &&value) {
            if (node == nullptr) {
                return new AVLNode(std::forward<TK>(key), std::forward<TV>(value));
            }

            if (node->key == key) {
                node->value = std::forward<TV>(value);
                return node;
            }

            if (key < node->key) {
                node->left = put(node->left, std::forward<TK>(key), std::forward<TV>(value));
            } else {
                node->right = put(node->right, std::forward<TK>(key), std::forward<TV>(value));
            }
            update_height(node);
            return balance(node);
        }

        // 从被删除节点开始，向上更新高度，并对每个失衡节点进行旋转，最多旋转次数为O(log(n))
        AVLNode *remove(AVLNode *node, const K &key, AVLNode *&del) {
            if (node == nullptr) {
                return nullptr;
            }

            if (key < node->key) {
                node->left = remove(node->left, key, del);
            } else if (node->key < key) {
                node->right = remove(node->right, key, del);
            } else {
                del = node;
                if (node->left == nullptr) {
                    node = node->right;
                } else if (node->right == nullptr) {
                    node = node->left;
                } else {
                    AVLNode *successor = min(node->right);
                    AVLNode *s_del;
                    successor->right = remove(node->right, successor->key, s_del);
                    successor->left = node->left;
                    node = successor;
                }
            }

            update_height(node);
            return balance(node);
        }
    };
}

int main() {
    AVLT::AVLTree<int, int> tree;
    tree.put(9, 9);
    tree.put(5, 5);
    tree.put(3, 3);
    tree.put(12, 12);
    tree.put(13, 13);

    tree.print();

    return 0;
}
