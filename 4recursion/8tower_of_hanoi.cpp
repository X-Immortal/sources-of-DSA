//
// Created by xyx on 25-7-26.
//
#include <iostream>
#include <list>

using namespace std;

ostream &operator<<(ostream &out, const list<int> &l) {
    out << "[";
    for (auto it = l.begin(); it != l.end(); ++it) {
        out << *it;
        if (it != --l.end()) {
            out <<  ", ";
        }
    }
    return out << "]";
}

// T(n) = 2T(n - 1) + c，T(1) = c
// T(n) = c(2^n - 1)
// 时间复杂度：O(2^n)
void move(int n, list<int> &from, list<int> &to, list<int> &aux) {
    if (n == 0) {
        return;
    }
    move(n - 1, from, aux, to);
    to.push_back(from.back());
    from.pop_back();
    cout << from << endl << aux << endl << to << endl;
    move(n - 1, aux, to, from);
}

int main() {
    list a = {3, 2, 1};
    list<int> b;
    list<int> c;
    move(3, a, c, b);
    return 0;
}