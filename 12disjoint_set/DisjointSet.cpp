//
// Created by xyx on 2025/12/13.
//
#include <iostream>

using std::cout;
using std::endl;

namespace DS {
    class DisjointSet {
        int *s;
        int m_size;

    public:
        DisjointSet(int size) : m_size(size) {
            s = new int[size];
            for (int i = 0; i < size; i++) {
                s[i] = i;
            }
        }

        ~DisjointSet() {
            delete[] s;
        }

        int find(int x) {
            if (x == s[x]) {
                return x;
            }
            return find(s[x]);
        }

        // 让两个集合“相交”
        void union2(int x, int y) {
            s[y] = x;
        }

    private:
    };
}