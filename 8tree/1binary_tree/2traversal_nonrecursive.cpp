//
// Created by xyx on 2025/10/1.
//
#include <iostream>
#include "TreeNode.h"
#include <stack>

using std::cout;
using std::endl;

enum Order {PRE, IN, POST};
struct Frame {
    TreeNode<int> *node;
    int pc;

    explicit Frame(TreeNode<int> *node) : node(node), pc(0) {}
};

// 模拟递归调用栈
void traversal1(TreeNode<int> *root, const Order order) {
    std::stack<Frame> s;
    s.emplace(root);
    while (!s.empty()) {
        Frame &cur = s.top();

        if (cur.node == nullptr) {
            s.pop();
            continue;
        }

        if (order == cur.pc) {
            cout << cur.node->val << " ";
        }

        switch (cur.pc) {
            case 0:
                s.emplace(cur.node->left);
                cur.pc++;
                break;
            case 1:
                s.emplace(cur.node->right);
                cur.pc++;
                break;
            case 2:
                s.pop();
                break;
            default:
                break;
        }
    }
}

enum State {RUN, OVER, UNKNOWN};

void traversal2(TreeNode<int> *root, const Order order) {
    std::stack<TreeNode<int> *> s;
    s.push(root);
    TreeNode<int> *last = nullptr;
    State state = RUN;
    State state_last = OVER;
    while (!s.empty()) {
        TreeNode<int> *cur = s.top();

        if (cur == nullptr) {
            last = cur;
            s.pop();
            state = OVER;
            continue;
        }

        switch (state) {
            case RUN:
                if (order == PRE) cout << cur->val << " ";
                s.emplace(cur->left);
                state_last = RUN;
                break;
            case OVER:
                if (state_last != RUN && cur->right == last) {
                    if (order == POST) cout << cur->val << " ";
                    last = cur;
                    s.pop();
                    state_last = UNKNOWN;
                } else {
                    if (order == IN) cout << cur->val << " ";
                    s.emplace(cur->right);
                    state = RUN;
                    state_last = OVER;
                }
                break;
            default: ;
        }
    }
}

int main() {
    TreeNode<int> *root = new TreeNode<int>(1);
    TreeNode<int> *n1 = new TreeNode<int>(2);
    TreeNode<int> *n2 = new TreeNode<int>(3);
    TreeNode<int> *n3 = new TreeNode<int>(4);
    TreeNode<int> *n4= new TreeNode<int>(5);
    TreeNode<int> *n5= new TreeNode<int>(6);
    root->left = n1, root->right = n2;
    n1->left = n3;
    n2->left = n4, n2->right = n5;

    traversal1(root, PRE);
    cout << endl;

    traversal1(root, IN);
    cout << endl;

    traversal1(root, POST);
    cout << endl;

    delete root, delete n1, delete n2, delete n3, delete n4, delete n5;
    return 0;
}