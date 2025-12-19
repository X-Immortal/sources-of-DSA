//
// Created by xyx on 2025/12/18.
//
#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>
#include <chrono>
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
    template<Comparable K, class V>
    class BTree {
        struct Node {
            // 孩子的数量比key的数量多1
            std::vector<std::pair<K, V>> pairs;
            std::vector<Node *> children;

            bool leaf() {
                return children.empty();
            }

            Node *get(const K &key) {
                int i = get_index(key);
                if (leaf() || (i < pairs.size() && pairs[i].first == key)) {
                    return this;
                }
                return children[i]->get(key);
            }

            int get_index(const K &key) {
                // pairs数组为升序，可用二分查找提高效率
                auto itr = std::lower_bound(pairs.begin(), pairs.end(), key,
                    [] (const std::pair<K, V> &a, const K &b) {
                        return a.first < b;
                    });
                return itr - pairs.begin();
            }

            Node *move_right_half(int from) {
                Node *new_node = new Node;
                new_node->pairs.assign(pairs.begin() + from, pairs.end());
                pairs.resize(from);
                if (!leaf()) {
                    new_node->children.assign(children.begin() + from, children.end());
                    children.resize(from);
                }
                return new_node;
            }

            void append(Node *node) {
                pairs.insert(pairs.end(), node->pairs.begin(), node->pairs.end());
                children.insert(children.end(), node->children.begin(), node->children.end());
            }

            template<typename T>
                requires std::constructible_from<std::pair<K, V>, T&&>
            void insert_pair(int index, T &&pair) {
                pairs.insert(pairs.begin() + index, std::forward<T>(pair));
            }

            template<typename T>
                requires std::constructible_from<std::pair<K, V>, T&&>
            void insert_first_pair(T &&pair) {
                insert_pair(0, std::forward<T>(pair));
            }

            template<typename T>
                requires std::constructible_from<std::pair<K, V>, T&&>
            void insert_last_pair(T &&pair) {
                pairs.push_back(std::forward<T>(pair));
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

            std::pair<K, V> remove_pair(int index) {
                auto ret = pairs[index];
                pairs.erase(pairs.begin() + index);
                return ret;
            }

            std::pair<K, V> remove_first_pair() {
                return remove_pair(0);
            }

            std::pair<K, V> remove_last_pair() {
                auto ret = pairs.back();
                pairs.pop_back();
                return ret;
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
            Node *node = root->get(key);
            int index = node->get_index(key);
            return index < node->pairs.size() && node->pairs[index].first == key;
        }

        V &get(const K &key) {
            Node *node = root->get(key);
            int index = node->get_index(key);
            if (index < node->pairs.size() && node->pairs[index].first == key) {
                return node->pairs[index].second;
            }

            throw std::runtime_error("key not found");
        }

        template<typename TK, typename TV>
            requires std::constructible_from<K, TK&&> && std::constructible_from<V, TV&&>
        void put(TK &&key, TV &&value) {
            put(root, std::forward<TK>(key), std::forward<TV>(value), nullptr, 0);
        }

        void remove(const K &key) {
            remove(root, key, nullptr, 0);
        }

        int greater_count(const K &key) {
            return greater_count(root, key);
        }

    private:
        int greater_count(Node *node, const K &key) {
            int index = node->get_index(key);
            if (index < node->pairs.size() && node->pairs[index].first == key) {
                ++index;
            }

            int count = node->pairs.size() - index;
            for (int i = index; i < node->children.size(); ++i) {
                count += greater_count(node->children[i], key);
            }
            return count;
        }

        void post_order(Node *node, const std::function<void(Node *)> &func) {
            for (auto child: node->children) {
                post_order(child, func);
            }
            func(node);
        }

        template<typename TK, typename TV>
            requires std::constructible_from<K, TK&&> && std::constructible_from<V, TV&&>
        void put(Node *node, TK &&key, TV &&value, Node *parent, int index_in_parent) {
            int index = node->get_index(key);
            if (index < node->pairs.size() && node->pairs[index].first == key) {
                node->pairs[index].second = std::forward<TV>(value);
                return;
            }
            if (node->leaf()) {
                node->insert_pair(index, std::make_pair(key, value));
            } else {
                // 从根向叶递归插入
                put(node->children[index], std::forward<TK>(key), std::forward<TV>(value), node, index);
            }

            // 从叶向根递归分裂
            if (overflow(node)) {
                split(node, parent, index_in_parent);
            }
        }

        bool overflow(Node *node) {
            return node->pairs.size() > MAX_KEY_NUM;
        }

        bool underflow(Node *node) {
            return node->pairs.size() < MIN_KEY_NUM;
        }

        bool enough(Node *node) {
            return node->pairs.size() > MIN_KEY_NUM;
        }

        // 将节点一分为三
        void split(Node *node, Node *parent, int index) {
            if (node == root) {
                Node *new_root = new Node;
                new_root->insert_child(index = 0, node);
                root = parent = new_root;
            }

            // 后半部分成为右侧新兄弟，前半部分保持不变
            Node *new_node = node->move_right_half(min_degree); // 新节点和原节点在同一层
            parent->insert_child(index + 1, new_node);
            // 中间元素并入父节点
            parent->insert_pair(index, node->remove_last_pair());
        }

        void remove(Node *node, const K &key, Node *parent, int index_in_parent) {
            int index = node->get_index(key);
            if (node->leaf()) {
                if (index < node->pairs.size() && node->pairs[index].first == key) {
                    // 叶节点：直接删除
                    node->remove_pair(index);
                } else {
                    return;
                }
            } else {
                if (index < node->pairs.size() && node->pairs[index].first == key) {
                    // 非叶节点：将当前key替换为后继key，再递归删除后继key
                    Node *succ = min(node->children[index + 1]);
                    node->pairs[index] = succ->pairs[0];
                    remove(node->children[index + 1], succ->pairs[0].first, node, index + 1);
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
                if (!node->leaf() && node->pairs.empty()) {
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
            left->insert_last_pair(node->pairs[index]);
            node->pairs[index] = right->remove_first_pair();
            if (!right->leaf()) {
                left->insert_last_child(right->remove_first_child());
            }
        }

        void right_rotate(Node *node, int index) {
            Node *left = node->children[index];
            Node *right = node->children[index + 1];
            right->insert_first_pair(node->pairs[index]);
            node->pairs[index] = left->remove_last_pair();
            if (!left->leaf()) {
                right->insert_first_child(left->remove_last_child());
            }
        }

        // 把父key、左孩子、右孩子合并
        void merge(Node *node, int index) {
            Node *left = node->children[index];
            Node *right = node->remove_child(index + 1);
            left->insert_last_pair(node->remove_pair(index));
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
    BT::BTree<int, int> tree(1000);

    auto start = std::chrono::duration_cast<std::chrono::microseconds>(
        std::chrono::system_clock::now().time_since_epoch()
    ).count();
    for (int i = 0; i < 100000; i++) {
        tree.put(i, i);
    }

    for (int i = 99999; i >= 0; i--) {
        try {
            auto v = tree.get(i);
            assert(v == i);
        } catch (std::runtime_error &) {
            assert(false);
        }
        assert(tree.greater_count(i) == 99999 - i);
    }

    for (int i = 0; i < 100000; i += 2) {
        tree.remove(i);
    }

    for (int i = 99999; i >= 0; i--) {
        try {
            auto v = tree.get(i);
            assert(i % 2);
            assert(v == i);
        } catch (std::runtime_error &) {
        }
    }
    auto end = std::chrono::duration_cast<std::chrono::microseconds>(
        std::chrono::system_clock::now().time_since_epoch()
    ).count();

    std::cout << "Time: " << (end - start) << "us" << std::endl;

    return 0;
}