//
// Created by xyx on 2025/10/11.
//
#include <iostream>
#include <string>
#include <functional>
#include <stack>

using std::cout;
using std::endl;

namespace BST {
    template<class K>
    concept Comparable = requires(const K &a, const K &b)
    {
        { a < b } -> std::convertible_to<bool>;
    };

    template<Comparable K, class V>
    class BinarySearchTree {
        struct BSTNode {
            // key是唯一的
            K key;
            V value;
            BSTNode *left;
            BSTNode *right;

            // 万能引用+完美转发
            template<class TK, class TV>
                requires std::constructible_from<K, TK&&> && std::constructible_from<V, TV&&>
            BSTNode(TK &&key, TV &&value, BSTNode *left = nullptr, BSTNode *right = nullptr) : key(std::forward<TK>(key)),
                value(std::forward<TV>(value)), left(left), right(right) {
            }
        };

        struct Frame {
            BSTNode *node;
            int pc;

            explicit Frame(BSTNode *node) : node(node), pc(0) {
            }
        };

        BSTNode *root;

    public:
        BinarySearchTree() : root(nullptr) {
        }

        ~BinarySearchTree() {
            post_order(root, [](BSTNode *node) {
                delete node;
            });
        }

        // 平均时间复杂度：O(log(n))
        // 最坏时间复杂度：O(n)，退化为链表
        V &get(const K &key) {
            BSTNode *cur = root;
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
            return min(root);
        }

        V &max() {
            return max(root);
        }

        template<class TK, class TV>
            requires std::constructible_from<K, TK&&> && std::constructible_from<V, TV&&>
        void put(TK &&key, TV &&value) {
            if (root == nullptr) {
                root = new BSTNode(std::forward<TK>(key), std::forward<TV>(value));
                return;
            }

            BSTNode *cur = root;
            BSTNode *parent = nullptr;
            while (cur != nullptr) {
                parent = cur;
                if (key < cur->key) {
                    cur = cur->left;
                } else if (cur->key < key) {
                    cur = cur->right;
                } else {
                    cur->value = std::forward<TV>(value);
                    return;
                }
            }

            if (key < parent->key) {
                parent->left = new BSTNode(std::forward<TK>(key), std::forward<TV>(value));
            } else {
                parent->right = new BSTNode(std::forward<TK>(key), std::forward<TV>(value));
            }
        }

        V &successor(const K &key) {
            BSTNode *node = root;
            BSTNode *ancestor = nullptr;
            while (node != nullptr) {
                if (key < node->key) {
                    ancestor = node;
                    node = node->left;
                } else if (node->key < key) {
                    node = node->right;
                } else {
                    break;
                }
            }
            if (node == nullptr) {
                throw std::runtime_error("key not found!");
            }

            if (node->right != nullptr) {
                return min(node->right);
            }

            if (ancestor != nullptr) {
                return ancestor->value;
            }

            throw std::runtime_error("The key doesn't have predecessor");
        }

        V &predecessor(const K &key) {
            BSTNode *node = root;
            BSTNode *ancestor = nullptr;
            while (node != nullptr) {
                if (key < node->key) {
                    node = node->left;
                } else if (node->key < key) {
                    ancestor = node;
                    node = node->right;
                } else {
                    break;
                }
            }
            if (node == nullptr) {
                throw std::runtime_error("key not found!");
            }

            if (node->left != nullptr) {
                return max(node->left);
            }

            if (ancestor != nullptr) {
                return ancestor->value;
            }

            throw std::runtime_error("The key doesn't have predecessor");
        }

        V del(const K &key) {
            BSTNode *node = root;
            BSTNode *parent = nullptr;
            while (node != nullptr) {
                if (key < node->key) {
                    parent = node;
                    node = node->left;
                } else if (node->key < key) {
                    parent = node;
                    node = node->right;
                } else {
                    break;
                }
            }

            if (node == nullptr) {
                throw std::runtime_error("key not found!");
            }

            if (node->left == nullptr) {
                shift(parent, node, node->right);
            } else if (node->right == nullptr) {
                shift(parent, node, node->left);
            } else {
                BSTNode *successor = node->right;
                BSTNode *s_parent = node;
                while (successor->left != nullptr) {
                    s_parent = successor;
                    successor = successor->left;
                }
                if (node != s_parent) {
                    // 后继节点的左子树为空
                    shift(s_parent, successor, successor->right);
                    successor->right = node->right;
                }
                shift(parent, node, successor);
                successor->left = node->left;
            }

            V ret = node->value;
            delete node;
            return ret;
        }

        V del_rec(const K &key) {
            BSTNode *node = nullptr;
            root = del_rec(root, key, node);
            if (node == nullptr) {
                throw std::runtime_error("key not found!");
            }

            V ret = node->value;
            delete node;
            return ret;
        }

        std::vector<V> less(const K &key) {
            std::vector<V> result;
            std::stack<Frame> s;
            s.emplace(root);

            while (!s.empty()) {
                Frame &frame = s.top();

                if (frame.node == nullptr) {
                    s.pop();
                    continue;
                }

                if (frame.pc == 0) {
                    s.emplace(frame.node->left);
                    ++frame.pc;
                } else if (frame.pc == 1) {
                    if (frame.node->key < key) {
                        result.push_back(frame.node->value);
                    } else {
                        break;
                    }
                    s.emplace(frame.node->right);
                    ++frame.pc;
                } else if (frame.pc == 2) {
                    s.pop();
                }
            }
            return result;
        }

        std::vector<V> greater(const K &key) {
            std::vector<V> result;
            std::stack<Frame> s;
            s.emplace(root);

            while (!s.empty()) {
                Frame &frame = s.top();

                if (frame.node == nullptr) {
                    s.pop();
                    continue;
                }

                if (frame.pc == 0) {
                    s.emplace(frame.node->right);
                    ++frame.pc;
                } else if (frame.pc == 1) {
                    if (key < frame.node->key) {
                        result.push_back(frame.node->value);
                    } else {
                        break;
                    }
                    s.emplace(frame.node->left);
                    ++frame.pc;
                } else if (frame.pc == 2) {
                    s.pop();
                }
            }
            return result;
        }

        std::vector<V> between(const K &key1, const K &key2) {
            std::vector<V> result;
            std::stack<Frame> s;
            s.emplace(root);

            while (!s.empty()) {
                Frame &frame = s.top();

                if (frame.node == nullptr) {
                    s.pop();
                    continue;
                }

                if (frame.pc == 0) {
                    s.emplace(frame.node->left);
                    ++frame.pc;
                } else if (frame.pc == 1) {
                    if (key1 <= frame.node->key && frame.node->key <= key2) {
                        result.push_back(frame.node->value);
                    } else if (key2 < frame.node->key) {
                        break;
                    }
                    s.emplace(frame.node->right);
                    ++frame.pc;
                } else if (frame.pc == 2) {
                    s.pop();
                }
            }
            return result;
        }

        void print() {
            post_order(root, [](BSTNode *node) {
                cout << node->key << ": " << node->value << endl;
            });
        }

    private:
        void post_order(BSTNode *node, const std::function<void(BSTNode *)> &func) {
            if (node == nullptr) {
                return;
            }
            post_order(node->left, func);
            post_order(node->right, func);
            func(node);
        }

        V &max(BSTNode *root) {
            if (root == nullptr) {
                throw std::runtime_error("tree is empty!");
            }
            while (root->right != nullptr) {
                root = root->right;
            }
            return root->value;
        }

        V &min(BSTNode *root) {
            if (root == nullptr) {
                throw std::runtime_error("tree is empty!");
            }
            while (root->left != nullptr) {
                root = root->left;
            }
            return root->value;
        }

        void shift(BSTNode *parent, BSTNode *node, BSTNode *child) {
            if (parent == nullptr) {
                root = child;
                return;
            }

            if (node == parent->left) {
                parent->left = child;
            } else {
                parent->right = child;
            }
        }

        BSTNode *del_rec(BSTNode *root, const K &key, BSTNode *&node) {
            if (root == nullptr) {
                return nullptr;
            }

            if (key < root->key) {
                root->left = del_rec(root->left, key, node);
                return root;
            }
            if (root->key < key) {
                root->right = del_rec(root->right, key, node);
                return root;
            }

            node = root;
            if (root->left == nullptr) {
                return root->right;
            }
            if (root->right == nullptr) {
                return root->left;
            }

            BSTNode *successor = root->right;
            while (successor->left != nullptr) {
                successor = successor->left;
            }
            BSTNode *s_node;
            successor->right = del_rec(root->right, successor->key, s_node);
            successor->left = root->left;
            return successor;
        }
    };
}

int main() {
    BST::BinarySearchTree<int, std::string> tree;
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
