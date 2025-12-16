//
// Created by xyx on 2025/9/28.
//
#include <iostream>
#include "TreeNode.h"

using std::cout;
using std::endl;

// 前序遍历
void preOrder(TreeNode<int> *root) {
    if (root == nullptr) {
        return;
    }
    cout << root->val << " ";
    preOrder(root->left);
    preOrder(root->right);
}

void inOrder(TreeNode<int> *root) {
    if (root == nullptr) {
        return;
    }
    inOrder(root->left);
    cout << root->val << " ";
    inOrder(root->right);
}

void postOrder(TreeNode<int> *root) {
    if (root == nullptr) {
        return;
    }
    postOrder(root->left);
    postOrder(root->right);
    cout << root->val << " ";
}

int main() {
    TreeNode<int> *root = new TreeNode(1);
    TreeNode<int> *n1 = new TreeNode(2);
    TreeNode<int> *n2 = new TreeNode(3);
    TreeNode<int> *n3 = new TreeNode(4);
    TreeNode<int> *n4= new TreeNode(5);
    TreeNode<int> *n5= new TreeNode(6);
    root->left = n1, root->right = n2;
    n1->left = n3;
    n2->left = n4, n2->right = n5;

    preOrder(root);
    cout << endl;

    inOrder(root);
    cout << endl;

    postOrder(root);
    cout << endl;

    delete root, delete n1, delete n2, delete n3, delete n4, delete n5;
    return 0;
}
