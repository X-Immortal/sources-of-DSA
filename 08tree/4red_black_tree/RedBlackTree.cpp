//
// Created by xyx on 2025/12/10.
//
#include <iostream>

using std::cout;
using std::endl;

namespace RBT {
    template<class K>
    concept Comparable = requires(const K &a, const K &b)
    {
        { a < b } -> std::convertible_to<bool>;
    };

    /*
     * 红黑规则：
     * 1. 叶子节点为空节点（而不是没有孩子的节点），空节点为黑色
     * 2. 红色节点不能相邻
     * 3. 根节点为黑色
     * 4. 对于任意子树，从根节点到到任意一个叶子节点，路径中的黑色节点数一样（黑色完美平衡）
     *
     * 由规则4可以推出：黑色节点只能成对出现，不可能存在一个黑色节点的兄弟为空，但可以有红色兄弟
     */
    template<Comparable K, class V>
    class RedBlackTree {
        enum Color { RED, BLACK };

        struct Node {
            K key;
            V value;
            Node *left;
            Node *right;
            Node *parent;
            Color color = RED; // 新节点为红色

            template<typename TK, typename TV>
                requires std::constructible_from<K, TK &&> && std::constructible_from<V, TV &&>
            Node(TK &&key, TV &&value, Node *parent, Node *left = nullptr, Node *right = nullptr) :
                key(std::forward<TK>(key)), value(std::forward<TV>(value)), parent(parent), left(left), right(right) {
            }

            bool is_left_child() {
                return parent != nullptr && parent->left == this;
            }

            // 叔叔节点：父节点的兄弟节点
            Node *uncle() {
                if (parent == nullptr) {
                    return nullptr;
                }
                return parent->sibling();
            }

            // 兄弟节点
            Node *sibling() {
                if (parent == nullptr) {
                    return nullptr;
                }
                if (is_left_child()) {
                    return parent->right;
                } else {
                    return parent->left;
                }
            }
        };

        Node *root = nullptr;

    public:
        template<typename TK, typename TV>
            requires std::constructible_from<K, TK &&> && std::constructible_from<V, TV &&>
        void put(TK &&key, TV &&value) {
            Node *cur = root;
            Node *parent = nullptr;
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

            Node *new_node = new Node(std::forward<TK>(key), std::forward<TV>(value), parent);
            if (parent == nullptr) {
                root = new_node;
            } else if (key < parent->key) {
                parent->left = new_node;
            } else {
                parent->right = new_node;
            }

            fix_red_red(new_node);
        }

        void remove(const K &key) {
            remove(get_node(key));
        }

        V &get(const K &key) {
            Node *node = get_node(key);
            if (node == nullptr) {
                throw std::out_of_range("key not found");
            }
            return node->value;
        }

    private:
        Node *get_node(const K &key) {
            Node *cur = root;
            while (cur != nullptr) {
                if (key < cur->key) {
                    cur = cur->left;
                } else if (cur->key < key) {
                    cur = cur->right;
                } else {
                    break;
                }
            }
            return cur;
        }

        bool is_red(Node *node) {
            return node != nullptr && node->color == RED;
        }

        // 空节点为黑色
        bool is_black(Node *node) {
            return node == nullptr || node->color == BLACK;
        }

        // 右旋
        void right_rotate(Node *node) {
            Node *root = node->left;
            Node *change = root->right;
            Node *parent = node->parent;

            node->left = change;
            if (change != nullptr) {
                change->parent = node;
            }

            if (parent == nullptr) {
                this->root = root;
            } else if (node->is_left_child()) {
                parent->left = root;
            } else {
                parent->right = root;
            }
            root->parent = parent;

            root->right = node;
            node->parent = root;
        }


        // 左旋
        void left_rotate(Node *node) {
            Node *root = node->right;
            Node *change = root->left;
            Node *parent = node->parent;

            node->right = change;
            if (change != nullptr) {
                change->parent = node;
            }

            if (parent == nullptr) {
                this->root = root;
            } else if (node->is_left_child()) {
                parent->left = root;
            } else {
                parent->right = root;
            }
            root->parent = parent;

            root->left = node;
            node->parent = root;
        }

        // 插入节点时，检查是否触发双红，即违反红色节点不能相邻的规则
        void fix_red_red(Node *node) {
            // 插入的节点为根节点，直接变为黑色
            if (node == root) {
                node->color = BLACK;
                return;
            }

            // 插入节点的父节点为黑色，无需调整
            if (is_black(node->parent)) {
                return;
            }

            // 插入节点的父节点为红色，违反了红色节点不能相邻的规则
            // 父节点为红色，说明父节点一定不是根节点，因此一定有祖父节点
            Node *parent = node->parent;
            Node *uncle = node->uncle();
            Node *grandparent = parent->parent;

            // 叔叔节点为红色
            /*
             * 只需变色，无需旋转
             * 1. 将父节点变黑，避免红红相邻
             * 2. 此时当前路径黑色节点多一个，为了保证黑色平衡，将叔叔节点也变黑
             * 3. 祖父一定为黑色，但此时祖父所在路径黑色节点过多，因此将祖父节点变红
             * 4. 祖父变红又违反了红红相邻，此时进行递归调整
             */
            if (is_red(uncle)) {
                // 叔叔节点为红色，说明叔叔节点不为空
                parent->color = BLACK;
                uncle->color = BLACK;
                grandparent->color = RED;
                fix_red_red(grandparent);
                return;
            }

            // 叔叔节点为黑色
            /*
             * 需要变色配合旋转
             * 与avl树相同，分成四种情况
             */
            if (parent->is_left_child()) {
                if (node->is_left_child()) {
                    /*
                     * LL
                     * 1. 将父节点变黑，避免红红相邻
                     * 2. 此时当前路径多1个黑色节点，而祖父一定为黑色，因此将祖父节点变红进行中和
                     * 3. 祖父变红导致叔叔节点所在路径黑色节点少1个，但叔叔节点本来就是黑色，无法变色调整
                     * 4. 以祖父节点为根进行右旋，使原本变黑的父节点同时处在当前路径和叔叔节点的路径，
                     *      保证当前路径黑色节点数量不变的同时为叔叔节点的路径增加一个黑色节点
                     * 5. 以叔叔节点为根的黑色深度为h，则以当前节点和兄弟节点为根的黑色深度均为h，
                     *      旋转后兄弟节点成为叔叔节点的新兄弟，两者保持黑色平衡
                     */
                    parent->color = BLACK;
                } else {
                    /*
                     * LR
                     * 1. 先以父节点为根进行左旋，转化为LL
                     * 2. 按照LL的方式处理
                     */
                    left_rotate(parent);
                    node->color = BLACK;
                }
                // 提取公共部分
                grandparent->color = RED;
                right_rotate(grandparent);
            } else {
                // 与上述情况对称
                if (!node->is_left_child()) {
                    // RR
                    parent->color = BLACK;
                } else {
                    // RL
                    right_rotate(parent);
                    node->color = BLACK;
                }
                grandparent->color = RED;
                left_rotate(grandparent);
            }
        }

        // 删除节点时，触发双黑，表示某条路径上缺少一个黑色节点
        void fix_black_black(Node *node) {
            if (node == root) {
                return;
            }

            Node *parent = node->parent;
            // 当前节点是黑色，一定有兄弟节点
            Node *sibling = node->sibling();

            // 被调整节点的兄弟节点为红色
            // 一定有两个黑色侄子节点，可通过旋转使黑色侄子成为新兄弟，转为其他情况
            if (is_red(sibling)) {
                if (node->is_left_child()) {
                    left_rotate(parent);
                } else {
                    right_rotate(parent);
                }
                // 更新颜色，暂时维持平衡
                parent->color = RED;
                sibling->color = BLACK;

                // 此处应更新指针
                parent = node->parent;
                sibling = node->sibling();
            }


            // 被调整节点兄弟为黑色
            /*
             * 当前节点为黑色叶节点，且兄弟节点为黑色，说明侄子节点要么是空节点，要么是红色叶节点
             * 递归过后，当前节点不再是叶节点，不满足上述规律
             */

            // 两个侄子节点均为黑色
            /*
             * 只需变色，无需旋转
             * 1. 将兄弟节点变红，使当前路径与兄弟节点路径黑色节点数量相同
             * 但此时父节点路径黑色节点数比其他路径少1个
             * 2. 若父节点是红色则直接变黑，既能补上黑节点，又避免了双红
             * 3. 若父节点为黑色，再次触发双黑，递归处理即可
             */
            if (is_black(sibling->left) && is_black(sibling->right)) {
                sibling->color = RED;
                if (is_red(parent)) {
                    parent->color = BLACK;
                } else {
                    fix_black_black(parent);
                }
                return;
            }


            // 至少有一个侄子节点为红色
            /*
             * 需要变色配合旋转
             * 与avl树相同，分成四种情况
             */
            if (sibling->is_left_child()) {
                if (is_red(sibling->left)) {
                    // LL
                    /*
                     * 1. 以父节点为根进行右旋，使黑色兄弟加入当前路径，补上当前路径缺少的1个黑节点
                     *  但父节点又离开了红色侄子路径，若父节点为黑色则侄子路径上又会缺少1个黑节点
                     * 2. 因此，兄弟节点（新根）应保持父节点原来的颜色，红色侄子应保持兄弟节点原来的颜色（变黑），
                     *      保证侄子路径上黑色节点数量不变
                     * 3. 可以认为父节点作为新节点加入了当前路径，将其变黑即可补上缺少的黑色节点
                     * 4. 以当前节点为根的黑色深度为h，则以兄弟节点为根的黑色深度为(h + 1)（因为当前路径缺少1个黑节点）
                     *      那么以侄子为根的黑色深度也为h，旋转后另一个侄子成为当前节点的兄弟，两者保持黑色平衡
                     */
                    sibling->color = parent->color;
                    sibling->left->color = BLACK;
                } else {
                    // LR
                    /*
                     * 通过旋转和变色转化为LL
                     * 1. 以兄弟节点为根进行左旋，则兄弟节点原位置的左子树黑色节点数不变，右子树黑色节点数减1
                     * 2. 为了维持黑色平衡，将红色侄子（新根）变黑，保持右子树黑色节点数不变
                     * 3. 但此时左子树黑色节点数多1个，因此将兄弟节点变红中和
                     * 4. 这样，原本的兄弟节点就成为了新的红色左侄子
                     * 换爹节点的黑色平衡略
                     *
                     * 右旋后的结果：
                     * 原兄弟节点位置不变，原红色侄子成为新根，原父节点降级
                     * 因此变色过程可以简化，红色侄子保持父节点原来的颜色，父节点变黑即可
                     */
                    sibling->right->color = parent->color;
                    left_rotate(sibling);
                }
                // 提取公共部分
                parent->color = BLACK;
                right_rotate(parent);
            } else {
                // 与上述情况对称
                if (is_red(sibling->right)) {
                    // RR
                    sibling->color = parent->color;
                    sibling->right->color = BLACK;
                } else {
                    sibling->left->color = parent->color;
                    right_rotate(sibling);
                }
                parent->color = BLACK;
                left_rotate(parent);
            }
        }

        void remove(Node *node) {
            if (node == nullptr) {
                return;
            }

            // 待删除节点有两个孩子，转化成其他情况
            if (node->left != nullptr && node->right != nullptr) {
                Node *cur = node->right;
                while (cur->left != nullptr) {
                    cur = cur->left;
                }
                // 只交换键值，而不是整个节点，可以简化操作
                std::swap(node->key, cur->key);
                std::swap(node->value, cur->value);
                node = cur;
            }

            // 待删除节点为根节点，单独处理
            if (node == root) {
                if (root->left == nullptr) {
                    root = root->right;
                } else {
                    root = root->left;
                }
                if (root != nullptr) {
                    // 根节点只有一个孩子，说明这个孩子一定是红色叶节点，直接调整属性即可
                    root->parent = nullptr;
                    root->color = BLACK;
                }
                delete node;
                return;
            }

            Node *parent = node->parent;

            // 删除叶节点
            if (node->left == nullptr && node->right == nullptr) {
                // 删除黑色叶节点，会导致当前路径缺少一个黑色节点，需要调整平衡
                if (is_black(node)) {
                    fix_black_black(node);
                }
                // 红色叶节点直接删除

                if (node->is_left_child()) {
                    parent->left = nullptr;
                } else {
                    parent->right = nullptr;
                }
                delete node;
                return;
            }

            // 删除带一个孩子的节点
            /*
             * 1. 如果红带红，违反红色节点不能相邻的规则，因此不可能
             * 2. 如果红带黑，一定违反黑色平衡，因此不可能
             * 3. 如果黑带黑，一定违反黑色平衡，因此不可能
             * 综上，此处只有可能是黑带红，且红色节点一定没有孩子
             */
            Node *replace;
            if (node->left == nullptr) {
                replace = node->right;
            } else {
                replace = node->left;
            }

            if (node->is_left_child()) {
                parent->left = replace;
            } else {
                parent->right = replace;
            }
            replace->parent = parent;

            /*
             * 删除黑色节点，但顶替上来的是红色节点，直接将红节点变黑即可保持黑色节点数不变
             */
            replace->color = BLACK;
            delete node;
        }
    };
}

int main() {
    RBT::RedBlackTree<int, int> tree;
    for (int i = 0; i < 10000; i++) {
        tree.put(i, i);
    }

    for (int i = 0; i < 10000; i++) {
        tree.remove(i);
    }

    return 0;
}
