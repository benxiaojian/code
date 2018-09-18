/*
  * ×Ö·û´®×ªÊý×Ö.c
 *
 *      Author: Nine
 */
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

// Insert a character
// Delete a character
// Replace a character

class Solution {
public:
    int minDistance(string word1, string word2) {
        int len1 = word1.size();
        int len2 = word2.size();

        vector<vector<int>> dp(len1+1, vector<int>(len2+1, 0));
        for(int i=0; i<=len1; ++i) {
            dp[i][0] = i;
        }

        for (int i=0; i<=len2; ++i) {
            dp[0][i] = i;
        }

        for (int i=1; i<=len1; ++i) {
            for (int j=1; j<=len2; ++j) {
                if (word1[i-1] == word2[j-1]) {
                    dp[i][j] = dp[i-1][j-1];
                } else {
                    dp[i][j] = 1 + min(min(dp[i-1][j-1], dp[i][j-1]), dp[i-1][j]); // Ìæ»»£¬É¾³ý£¬Ôö¼Ó
                    // dp[i][j] = 1 + min(dp[i][j-1]), dp[i-1][j]); // É¾³ý£¬Ôö¼Ó
                }
            }
        }

        return dp[len1][len2];
    }
};
