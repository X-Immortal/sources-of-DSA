//
// Created by xyx on 2025/12/19.
//
#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>
#include <functional>
#include <chrono>

using std::cout;
using std::endl;

namespace BPT {
    template<class K>
    concept Comparable = requires(const K &a, const K &b)
    {
        { a < b } -> std::convertible_to<bool>;
        { a == b } -> std::convertible_to<bool>;
    };

    // B+树与B-树的区别：
    // 1. B+树只有叶子节点存储value，非叶子节点只存储key；而B-树每个节点都存储value
    // 2. B+树非叶子节点中的key是叶子节点中key的副本；而B-树所有节点都有唯一的key
    // 3. B+树的所有叶子节点构成一个额外的链表，在范围查询时性能很高；而B-树的叶子节点是独立的，范围查询只能通过中序遍历
    // 4. B+树的各操作时间复杂度都稳定为O(logN)；而B-树可能达到O(1)
    template<Comparable K, class V>
    class BPlusTree {
        struct Node {
            // 孩子的数量比key的数量多1
            std::vector<K> keys;
            std::vector<V> values;
            std::vector<Node *> children;
            // 用于叶子节点链表
            Node *next;
            Node *prev;

            explicit Node(Node *next = nullptr, Node *prev = nullptr) : next(next), prev(prev) {}

            bool leaf() const {
                return children.empty();
            }

            Node *get(const K &key) {
                if (leaf()) {
                    return this;
                }
                return children[get_index(key)]->get(key);
            }

            int get_index(const K &key) const {
                // keys数组为升序，可用二分查找提高效率
                if (leaf()) {
                    auto itr = std::lower_bound(keys.begin(), keys.end(), key);
                    return itr - keys.begin();
                } else {
                    // 非叶子节点的key为副本，即使相等也应该跳过，继续在子树中查找
                    auto itr = std::upper_bound(keys.begin(), keys.end(), key);
                    return itr - keys.begin();
                }
            }

            Node *move_right_half(int from) {
                Node *new_node = new Node;
                new_node->keys.assign(keys.begin() + from, keys.end());
                keys.resize(from);
                if (!leaf()) {
                    new_node->children.assign(children.begin() + from, children.end());
                    children.resize(from);
                } else {
                    new_node->values.assign(values.begin() + from, values.end());
                    values.resize(from);
                    Node *next = this->next;
                    new_node->next = next;
                    new_node->prev = this;
                    this->next = new_node;
                    if (next != nullptr) {
                        next->prev = new_node;
                    }
                }
                return new_node;
            }

            void append(Node *node) {
                keys.insert(keys.end(), node->keys.begin(), node->keys.end());
                if (leaf()) {
                    values.insert(values.end(), node->values.begin(), node->values.end());
                    next = node->next;
                    if (next != nullptr) {
                        next->prev = this;
                    }
                } else {
                    children.insert(children.end(), node->children.begin(), node->children.end());
                }
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

            template<typename TV>
                requires std::constructible_from<V, TV&&>
            void insert_value(int index, TV &&value) {
                values.insert(values.begin() + index, std::forward<TV>(value));
            }

            template<typename TV>
                requires std::constructible_from<V, TV&&>
            void insert_first_value(TV &&value) {
                insert_value(0, std::forward<TV>(value));
            }

            template<typename TV>
                requires std::constructible_from<V, TV&&>
            void insert_last_value(TV &&value) {
                values.push_back(std::forward<TV>(value));
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
                K key = keys.back();
                keys.pop_back();
                return key;
            }

            V remove_value(int index) {
                V value = values[index];
                values.erase(values.begin() + index);
                return value;
            }

            V remove_first_value() {
                return remove_value(0);
            }

            V remove_last_value() {
                V value = values.back();
                values.pop_back();
                return value;
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
        ~BPlusTree() {
            post_order(root, [](Node *node) { delete node; });
        }

        explicit BPlusTree(int min_degree = 2) :
            min_degree(min_degree), MAX_KEY_NUM(2 * min_degree - 1), MIN_KEY_NUM(min_degree - 1) {
            if (min_degree < 2) {
                throw std::invalid_argument("min_degree must be >= 2");
            }
            root = new Node;
        }

        bool contains(const K &key) {
            Node *node = root->get(key);
            int index = node->get_index(key);
            return index < node->keys.size() && node->keys[index] == key;
        }

        V &get(const K &key) {
            Node *node = root->get(key);
            int index = node->get_index(key);
            if (index < node->keys.size() && node->keys[index] == key) {
                return node->values[index];
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
            Node *cur = root->get(key);
            int index = cur->get_index(key);
            if (index < cur->keys.size() && cur->keys[index] == key) {
                ++index;
            }

            int count = cur->keys.size() - index;
            cur = cur->next;
            while (cur != nullptr) {
                count += cur->keys.size();
                cur = cur->next;
            }
            return count;
        }

    private:
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
            if (index < node->keys.size() && node->keys[index] == key) {
                node->values[index] = std::forward<TV>(value);
                return;
            }
            if (node->leaf()) {
                node->insert_key(index, std::forward<TK>(key));
                node->insert_value(index, std::forward<TV>(value));
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

            // 后半部分成为右侧新兄弟，前半部分保持不变
            Node *new_node = node->move_right_half(min_degree); // 新节点和原节点在同一层
            parent->insert_child(index + 1, new_node);
            if (node->leaf()) {
                // 叶子节点，将右侧新兄弟第一个key复制到父节点
                parent->insert_key(index, new_node->keys[0]);
            } else {
                // 非叶子节点，处理方式与B-树相同，中间元素并入父节点
                parent->insert_key(index, node->remove_last_key());
            }
        }

        void remove(Node *node, const K &key, Node *parent, int index_in_parent) {
            int index = node->get_index(key);
            if (node->leaf()) {
                if (index < node->keys.size() && node->keys[index] == key) {
                    node->remove_key(index);
                    node->remove_value(index);
                } else {
                    return;
                }
            } else {
                // 只能在叶子节点中删除
                remove(node->children[index], key, node, index);
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
            if (left->leaf()) {
                // 涉及叶子节点
                // 将右孩子的第一个key挪到左孩子，父节点的key更新为右孩子第一个key
                left->insert_last_key(right->remove_first_key());
                left->insert_last_value(right->remove_first_value());
                node->keys[index] = right->keys[0];
            } else {
                // 不涉及叶子节点，处理方式与B-树相同
                left->insert_last_key(node->keys[index]);
                node->keys[index] = right->remove_first_key();
                left->insert_last_child(right->remove_first_child());
            }
        }

        void right_rotate(Node *node, int index) {
            Node *left = node->children[index];
            Node *right = node->children[index + 1];
            if (left->leaf()) {
                // 涉及叶子节点
                // 将左孩子的最后一个key挪到右孩子，父节点的key更新为右孩子第一个key
                right->insert_first_key(left->remove_last_key());
                right->insert_first_value(left->remove_last_value());
                node->keys[index] = right->keys[0];
            } else {
                // 不涉及叶子节点，处理方式与B-树相同
                right->insert_first_key(node->keys[index]);
                node->keys[index] = left->remove_last_key();
                right->insert_first_child(left->remove_last_child());
            }
        }

        // 把父key、左孩子、右孩子合并
        void merge(Node *node, int index) {
            Node *left = node->children[index];
            Node *right = node->remove_child(index + 1);
            if (left->leaf()) {
                // 涉及叶子节点
                // 父节点的key直接删除即可，无需并入左孩子
                node->remove_key(index);
            } else {
                // 不涉及叶子节点，处理方式与B-树相同
                left->insert_last_key(node->remove_key(index));
            }
            left->append(right);
            delete right;
        }
    };
}

int main() {
    BPT::BPlusTree<int, int> tree(1000);

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
