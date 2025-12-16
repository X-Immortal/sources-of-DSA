//
// Created by xyx on 2025/10/10.
//
#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include "TreeNode.h"

using namespace std;

TreeNode<string> *buildExpressionTree(const vector<string> &expr) {
    stack<TreeNode<string> *> stack;
    for (const string &str : expr) {
        if (str == "+" || str == "-" || str == "*" || str == "/") {
            if (stack.size() < 2) {
                throw runtime_error("error");
            }
            TreeNode<string> *right = stack.top();
            stack.pop();
            TreeNode<string> *left = stack.top();
            stack.pop();
            TreeNode<string> *sub_tree = new TreeNode(str, left, right);
            stack.push(sub_tree);
        } else {
            stack.push(new TreeNode(str));
        }
    }
    return stack.top();
}

void free_tree(TreeNode<string> *root) {
    if (root == nullptr) {
        return;
    }
    free_tree(root->left);
    free_tree(root->right);
    cout << root->val << " ";
    delete root;
}

int main() {
    free_tree(buildExpressionTree({"2", "1", "-", "3", "*"}));
    return 0;
}