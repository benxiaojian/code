/*
 * еп╤о╩ьнд
 *
 *      Author: Nine
 */

#include <iostream>
#include <string>
#include <sstream>
#include <cstdio>
#include <cstring>
using namespace std;


class Solution {
public:
    bool isPalindrome(int x) {
        stringstream stream;
        string str;
        bool is_palindrome = true;
        int i,j;

        stream << x;
        str = stream.str();

        for (i=0, j=str.length()-1; i<=j; i++, j--) {
            if (str[i] != str[j]) {
                is_palindrome = false;
                break;
            }
        }

        return is_palindrome;
    }
};


int main()
{
    Solution s;
    cout << s.isPalindrome(121) << endl;
}

