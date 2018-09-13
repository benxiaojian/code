/*
  * ���ظ����ַ���.c
 *
 *      Author: Nine
 */
#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>
#include <cstring>
using namespace std;

// �û�������ȥ����һ�������ظ��ģ��ƶ�������ߣ�֪��û���ظ�

class Solution {
public:
    int lengthOfLongestSubstring(string s) {
        int max = 0;
        int characters[256] = {0};
        int sum = 0;
        int i,j;

        for (i = 0, j = 0; j < s.length();) {
            int c = s[j];

            if (characters[c] == 0) {
                sum++;
                characters[c]++;
                j++;
            } else {
                if (max < sum) {
                    max = sum;
                }

                // remove left chart, until not repeat
                if (characters[c] > 0) {
                    characters[s[i]]--;
                    i++;
                    sum--;
                }
            }
        }

        if (max < sum) {
            return sum;
        }

        return max;
    }

};


int main()
{
    // dvfdf

    Solution s;

    cout << s.lengthOfLongestSubstring("dvfdf") << endl;
}
