//
// Created by xyx on 2025/10/1.
//

#ifndef ONLINE_CLASS_TREENODE_H
#define ONLINE_CLASS_TREENODE_H

template<class T>
struct TreeNode {
    T val;
    TreeNode *left;
    TreeNode *right;

    explicit TreeNode(T val, TreeNode *left = nullptr, TreeNode *right = nullptr):
    val(val), left(left), right(right) {}
};

#endif //ONLINE_CLASS_TREENODE_H