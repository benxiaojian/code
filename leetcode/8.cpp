/*
  * 字符串转数字.c
 *
 *      Author: Nine
 */
#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>
#include <cstring>
#include <climits>
using namespace std;

// 移除开头空格
// 开头只能是+,-,number
// number后面字符忽略
// number前不能有字符
// INT_MAX = 2147483647
// INT_MIN = -2147483648

class Solution {
private:
    int calcNumber(int res, int num, int plus_or_minus)
    {
        if (res == 0) {
            res = plus_or_minus * num;
            return res;
        }

        if ((res == INT_MAX) || (res > INT_MAX / 10) || ((res == (INT_MAX) / 10) && (num > 7))) {
            return INT_MAX;
        }

        if ((res == INT_MIN) || (res < INT_MIN / 10) || ((res == (INT_MIN) / 10) && (num > 8))) {
            return INT_MIN;
        }

        return res * 10 + num * plus_or_minus;
    }


    const char *find_first_number(const char *str, int *plus_or_minus, int str_len)
    {
        for (int i=0; i<str_len; ++i) {
            if (str[i] == '+') {
                *plus_or_minus = 1;
                return str + i + 1;
            } else if (str[i] == '-'){
                *plus_or_minus = -1;
                return str + i + 1;
            } else if (isdigit(str[i])) {
                return str + i;
            } else if (str[i] == ' ') {
                continue;
            } else {
                return NULL;
            }
        }

        return NULL;
    }

    int getNumber(const char *str, int plus_or_minus)
    {
        int res = 0;

        for (int i=0; i<strlen(str); ++i) {
            if (isdigit(str[i]) == false) {
                return res;
            }

            res = calcNumber(res, str[i] - '0', plus_or_minus);
        }

        return res;
    }

public:
    int myAtoi(string str) {
        const char *number_string = str.c_str();
        const char *first_number  = NULL;
        bool find_number = false;
        int res = 0;
        bool find_plus_or_minus = false;
        int plus_or_minus = 1;

        first_number = find_first_number(number_string, &plus_or_minus, str.length());
        if (first_number == NULL) {
            return 0;
        }

        res = getNumber(first_number, plus_or_minus);
        return res;
    }
};


int main()
{
    Solution s;

    if (s.myAtoi("9223372036854775808") == 2147483647) {
        cout << "true" << endl;
    }

    if (s.myAtoi("-91283472332") == -2147483648) {
            cout << "true" << endl;
        }

    if (s.myAtoi("-42") == -42) {
            cout << "true" << endl;
        }

    if (s.myAtoi("") == -42) {
            cout << "true" << endl;
        }
}
