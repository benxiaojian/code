/*
 * hd_1003.cpp
 *
 *      Author: Nine
 */


// 动态规划 求最大子序列

#include <iostream>
#include <cstdio>
#include <cstring>
using namespace std;


int main()
{
    int t;
    int n;
    int dp[100001];

    while (~scanf("%d", &t)) {
        for (int tt=1; tt<=t; ++tt){
            memset(dp, 0, sizeof(dp));
            int max = -99999;
            int end = 1;
            int begin = 1;
            int a[100001];

            scanf("%d", &n);
            for (int i=1; i<=n; ++i) {
                scanf("%d", &a[i]);

                if (dp[i-1] + a[i]>= a[i]) {
                    dp[i] = dp[i-1] + a[i];
                } else {
                    dp[i] = a[i];
                }
            }

            // find max and end index
            for (int i=1; i<=n; ++i) {
                if (dp[i] > max) {
                    max = dp[i];
                    end = i;
                }
            }

            // find begin index
            int sum = max;
            for (int i=end; i>0; --i) {
                sum -=  a[i];
                if (sum == 0) {
                    begin = i;
                }
            }

            printf("Case %d:\n", tt);
            printf("%d %d %d\n", max, begin, end);
            if (tt != t) {
                printf("\n");
            }
        }
    }
}
