/*
 * 罗马数字转数字
 *
 *      Author: Nine
 */

#include <iostream>
#include <string>
#include <map>
using namespace std;


class Solution {
private:
    void initRomanNumber(void)
    {
        this->roman_map.insert(pair<char, int>('I', 1));
        this->roman_map.insert(pair<char, int>('V', 5));
        this->roman_map.insert(pair<char, int>('X', 10));
        this->roman_map.insert(pair<char, int>('L', 50));
        this->roman_map.insert(pair<char, int>('C', 100));
        this->roman_map.insert(pair<char, int>('D', 500));
        this->roman_map.insert(pair<char, int>('M', 1000));
    }
public:
    int romanToInt(string s) {
        int prev_number = 0;
        int res = 0;

        initRomanNumber();

        for (int i=0; i<s.length(); ++i) {
            if ((5 * roman_map[s[i]] == roman_map[s[i+1]]) || (10 * roman_map[s[i]] == roman_map[s[i+1]]) && (i < s.length() - 1)) {
                res -= roman_map[s[i]];
            } else {
                res += roman_map[s[i]];
            }
        }

        return res;
    }
private:
    map<char, int> roman_map;
};
