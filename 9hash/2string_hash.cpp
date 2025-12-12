//
// Created by xyx on 2025/12/11.
//
#include <iostream>
#include <string>
#include <functional>

using std::cout;
using std::endl;

namespace Hash {
    // 哈希算法/摘要算法/散列算法
    // Java的String类中，哈希函数的实现
    // 将每一个字符的ASCII码按一定权重累加作为整个字符串的哈希码
    int hash(const std::string &str) {
        int hash = 0;
        for (char c : str) {
            // hash = hash * 31 + c
            // 以质数为权重，可减少哈希冲突
            hash = (hash << 5) - hash + c;
        }
        return hash;
    }
}

int main() {
    std::string str = "abc";
    std::string str2 = "bca";
    // Java的哈希函数
    cout << Hash::hash(str) << endl;
    cout << Hash::hash(str2) << endl;
    // C++的哈希函数
    cout << std::hash<std::string>()(str) << endl;
    cout << std::hash<std::string>()(str2) << endl;
    return 0;
}