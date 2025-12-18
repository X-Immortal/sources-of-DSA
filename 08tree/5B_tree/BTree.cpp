//
// Created by xyx on 2025/12/18.
//
#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>
#include <functional>

using std::cout;
using std::endl;

namespace BT {
    template<class K>
    concept Comparable = requires(const K &a, const K &b)
    {
        { a < b } -> std::convertible_to<bool>;
        { a == b } -> std::convertible_to<bool>;
    };

    // 度数(degree)：节点的孩子数量
    // 阶数(order)：节点的最大孩子数
    // 非叶非根节点的min_degree = ceil(order / 2)
    template<Comparable K>
    class BTree {
        struct Node {
            // 孩子的数量比key的数量多1
            std::vector<K> keys;
            std::vector<Node *> children;

            bool leaf() {
                return children.empty();
            }

            Node *get(const K &key) {
                int i = get_index(key);
                if (i < keys.size() && keys[i] == key) {
                    return this;
                }
                if (leaf()) {
                    return nullptr;
                }
                return children[i]->get(key);
            }

            int get_index(const K &key) {
                // keys数组为升序，可用二分查找提高效率
                auto itr = std::lower_bound(keys.begin(), keys.end(), key);
                return itr - keys.begin();
            }

            Node *move_right_half(int from) {
                Node *new_node = new Node;
                new_node->keys.assign(keys.begin() + from, keys.end());
                keys.resize(from);
                if (!leaf()) {
                    new_node->children.assign(children.begin() + from, children.end());
                    children.resize(from);
                }
                return new_node;
            }

            void append(Node *node) {
                keys.insert(keys.end(), node->keys.begin(), node->keys.end());
                children.insert(children.end(), node->children.begin(), node->children.end());
            }

            template<typename TK>
                requires std::constructible_from<K, TK&&>
            void insert_key(int index, TK &&key) {
                keys.insert(keys.begin() + index, std::forward<TK>(key));
            }

            template<typename TK>
                requires std::constructible_from<K, TK&&>
            void insert_first_key(TK &&key) {
                insert_key(0, std::forward<TK>(key));
            }

            template<typename TK>
                requires std::constructible_from<K, TK&&>
            void insert_last_key(TK &&key) {
                keys.push_back(std::forward<TK>(key));
            }

            void insert_child(int index, Node *child) {
                children.insert(children.begin() + index, child);
            }

            void insert_first_child(Node *child) {
                insert_child(0, child);
            }

            void insert_last_child(Node *child) {
                children.push_back(child);
            }

            K remove_key(int index) {
                K key = keys[index];
                keys.erase(keys.begin() + index);
                return key;
            }

            K remove_first_key() {
                return remove_key(0);
            }

            K remove_last_key() {
                return remove_key(keys.size() - 1);
            }

            Node *remove_child(int index) {
                Node *child = children[index];
                children.erase(children.begin() + index);
                return child;
            }

            Node *remove_first_child() {
                return remove_child(0);
            }

            Node *remove_last_child() {
                return remove_child(children.size() - 1);
            }

            Node *child_left_sibling(int index) {
                return index > 0 ? children[index - 1] : nullptr;
            }

            Node *child_right_sibling(int index) {
                return index < children.size() - 1 ? children[index + 1] : nullptr;
            }
        };

        Node *root;
        const int min_degree;
        const int MAX_KEY_NUM;
        const int MIN_KEY_NUM;

    public:
        ~BTree() {
            post_order(root, [](Node *node) { delete node; });
        }

        explicit BTree(int min_degree = 2) :
            min_degree(min_degree), MAX_KEY_NUM(2 * min_degree - 1), MIN_KEY_NUM(min_degree - 1) {
            if (min_degree < 2) {
                throw std::invalid_argument("min_degree must be >= 2");
            }
            root = new Node;
        }

        bool contains(const K &key) {
            return root->get(key) != nullptr;
        }

        template<typename TK>
            requires std::constructible_from<K, TK&&>
        void put(TK &&key) {
            put(root, std::forward<TK>(key), nullptr, 0);
        }

        void remove(const K &key) {
            remove(root, key, nullptr, 0);
        }

    private:
        void post_order(Node *node, const std::function<void(Node *)> &func) {
            for (auto child: node->children) {
                post_order(child, func);
            }
            func(node);
        }

        template<typename TK>
            requires std::constructible_from<K, TK&&>
        void put(Node *node, TK &&key, Node *parent, int index_in_parent) {
            int index = node->get_index(key);
            if (index < node->keys.size() && node->keys[index] == key) {
                return;
            }
            if (node->leaf()) {
                node->insert_key(index, std::forward<TK>(key));
            } else {
                // 从根向叶递归插入
                put(node->children[index], std::forward<TK>(key), node, index);
            }

            // 从叶向根递归分裂
            if (overflow(node)) {
                split(node, parent, index_in_parent);
            }
        }

        bool overflow(Node *node) {
            return node->keys.size() > MAX_KEY_NUM;
        }

        bool underflow(Node *node) {
            return node->keys.size() < MIN_KEY_NUM;
        }

        bool enough(Node *node) {
            return node->keys.size() > MIN_KEY_NUM;
        }

        // 将节点一分为三
        void split(Node *node, Node *parent, int index) {
            if (node == root) {
                Node *new_root = new Node;
                new_root->insert_child(index = 0, node);
                root = parent = new_root;
            }

            // 中间元素并入父节点
            parent->insert_key(index, node->keys[min_degree - 1]);
            // 后半部分成为右侧新兄弟，前半部分保持不变
            Node *new_node = node->move_right_half(min_degree); // 新节点和原节点在同一层
            parent->insert_child(index + 1, new_node);
            node->keys.resize(min_degree - 1);
        }

        void remove(Node *node, const K &key, Node *parent, int index_in_parent) {
            int index = node->get_index(key);
            if (node->leaf()) {
                if (index < node->keys.size() && node->keys[index] == key) {
                    // 叶节点：直接删除
                    node->remove_key(index);
                } else {
                    return;
                }
            } else {
                if (index < node->keys.size() && node->keys[index] == key) {
                    // 非叶节点：将当前key替换为后继key，再递归删除后继key
                    Node *succ = min(node->children[index + 1]);
                    node->keys[index] = succ->keys[0];
                    remove(node->children[index + 1], succ->keys[0], node, index + 1);
                } else {
                    // 递归向下删除
                    remove(node->children[index], key, node, index);
                }
            }

            // 递归向上调整
            balance(node, parent, index_in_parent);
        }

        void balance(Node *node, Node *parent, int index_in_parent) {
            if (!underflow(node)) {
                return;
            }

            if (node == root) {
                // 若根节点为空且有孩子，则让孩子成为新根
                if (!node->leaf() && node->keys.empty()) {
                    root = node->children[0];
                    delete node;
                }
                // 根节点可以无视下溢
                return;
            }

            Node *left = parent->child_left_sibling(index_in_parent);
            Node *right = parent->child_right_sibling(index_in_parent);
            // 左边兄弟富裕，右旋
            if (left != nullptr && enough(left)) {
                right_rotate(parent, index_in_parent - 1);
                return;
            }

            // 右边兄弟富裕，左旋
            if (right != nullptr && enough(right)) {
                left_rotate(parent, index_in_parent);
                return;
            }

            // 两边都不够，合并
            if (left != nullptr) {
                // 与左兄弟合并
                merge(parent, index_in_parent - 1);
            } else {
                // 与右兄弟合并
                merge(parent, index_in_parent);
            }
        }

        void left_rotate(Node *node, int index) {
            Node *left = node->children[index];
            Node *right = node->children[index + 1];
            left->insert_last_key(node->keys[index]);
            node->keys[index] = right->remove_first_key();
            if (!right->leaf()) {
                left->insert_last_child(right->remove_first_child());
            }
        }

        void right_rotate(Node *node, int index) {
            Node *left = node->children[index];
            Node *right = node->children[index + 1];
            right->insert_first_key(node->keys[index]);
            node->keys[index] = left->remove_last_key();
            if (!left->leaf()) {
                right->insert_first_child(left->remove_last_child());
            }
        }

        // 把父key、左孩子、右孩子合并
        void merge(Node *node, int index) {
            Node *left = node->children[index];
            Node *right = node->remove_child(index + 1);
            left->insert_last_key(node->remove_key(index));
            left->append(right);
            delete right;
        }

        Node *min(Node *node) {
            while (!node->leaf()) {
                node = node->children[0];
            }
            return node;
        }
    };
}

int main() {
    BT::BTree<int> tree(1000);

    for (int i = 0; i < 100000; i++) {
        tree.put(i);
    }

    for (int i = 99999; i >= 0; i--) {
        assert(tree.contains(i));
    }

    for (int i = 0; i < 100000; i += 2) {
        tree.remove(i);
    }

    for (int i = 99999; i >= 0; i--) {
        assert(tree.contains(i) == i % 2);
    }

    return 0;
}