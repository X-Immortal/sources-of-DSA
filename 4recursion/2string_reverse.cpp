//
// Created by xyx on 25-7-25.
//
#include <iostream>
#include <string>

using namespace std;

void reverse(const string &str, int start) {
    if (start == str.size()) {
        return;
    }
    reverse(str, start + 1);
    cout << str[start];
}

int main() {
    reverse("abcd", 0);
    return 0;
}