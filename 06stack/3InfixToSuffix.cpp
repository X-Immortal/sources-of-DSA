//
// Created by xyx on 2025/9/16.
//
#include <iostream>
#include <stack>
#include <map>
#include <string>
#include <sstream>

using std::cout;
using std::endl;

std::map<char, int> priority = {
    {'+', 1},
    {'-', 1},
    {'*', 2},
    {'/', 2},
    {'(', 0}
};

std::string infixToSuffix(const std::string &expr) {
    std::stack<char> operators;
    std::stringstream suffix;
    for (char token: expr) {
        switch (token) {
            case '+':
            case '-':
            case '*':
            case '/':
                while (!operators.empty() &&
                       priority[operators.top()] >= priority[token]) {
                    suffix << operators.top();
                    operators.pop();
                }
                operators.push(token);
            case '(':
                operators.push(token);
                break;
            case ')':
                while (operators.top() != '(') {
                    suffix << operators.top();
                    operators.pop();
                }
                operators.pop();
                break;
            default:
                suffix << token;
                break;
        }
    }
    while (!operators.empty()) {
        suffix << operators.top();
        operators.pop();
    }
    return suffix.str();
}

int main() {
    cout << infixToSuffix("a+b") << endl;
    cout << infixToSuffix("a+b-c") << endl;
    cout << infixToSuffix("a+b*c") << endl;
    cout << infixToSuffix("a*b-c") << endl;
    cout << infixToSuffix("a*(b+c)") << endl;
    cout << infixToSuffix("(a+b)*c") << endl;
    cout << infixToSuffix("(a+b*c-d)*e") << endl;
    return 0;
}
