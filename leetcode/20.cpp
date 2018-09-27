/*
 * two_sum.c
 *
 *      Author: Nine
 */
#include <iostream>
#include <stack>
#include <map>
#include <stdio.h>
#include <stdlib.h>
using namespace std;


class Solution {
public:
    bool isValid(string s) {
        stack<char> stack;
        map<char, char> p = { { ')' , '(' },
                { ']' , '[' },
                { '}' , '{' }};


        for (int i=0; i<s.length(); ++i) {
            if (s[i] == '(' || s[i] == '[' || s[i] == '{') {
                stack.push(s[i]);
            } else {
                if (stack.empty()) {
                    return false;
                }
                char c = stack.top();
                if (c != p[s[i]]) {
                    return false;
                }

                stack.pop();
            }
        }

        if (!stack.empty()) {
           return false;
        }

        return true;
    }
};


class Solution2 {
public:
    bool isValid(string s) {
        stack<char> stack;

        for (char &c : s) {
            if (c == '(') {
                stack.push(')');
            } else if (c == '[') {
                stack.push(']');
            } else if (c == '{') {
                stack.push('}');
            } else {
                if (stack.empty() || stack.top() != c) {
                    return false;
                }
                stack.pop();
            }
        }

        return stack.empty();
};


int main()
{
    Solution s;

    cout << s.isValid("()") << endl;
}
