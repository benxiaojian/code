/*
 * hd_1058.cpp
 *
 *      Author: Nine
 */


// DP

#include <iostream>
#include <cstdio>
#include <cstring>
using namespace std;

long long dp[5842] = {0};


void getAllNumber()
{
    long long number;
    int n2,n3,n5,n7;

    n2=n3=n5=n7=1;
    dp[1] = 1;

    for (int i=2; i<=5842; ++i) {
        number = min(min(min(dp[n2]*2, dp[n3]*3), dp[n5]*5), dp[n7]*7);
        if (number == dp[n2] * 2) {
            n2++;
        }

        if (number == dp[n3] * 3) {
            n3++;
        }

        if (number == dp[n5] * 5) {
            n5++;
        }

        if (number == dp[n7] * 7) {
            n7++;
        }

        dp[i] = number;
    }
}


int main()
{
    int number;

    getAllNumber();

    while (scanf("%d", &number) && number != 0) {
        if((1==number%10)&&(11!=number%100))
            cout<<"The "<<number<<"st humble number is "<<dp[number]<<"."<<endl;
        else if((2==number%10)&&(12!=number%100))
            cout<<"The "<<number<<"nd humble number is "<<dp[number]<<"."<<endl;
        else if((3==number%10)&&(13!=number%100))
            cout<<"The "<<number<<"rd humble number is "<<dp[number]<<"."<<endl;
        else
            cout<<"The "<<number<<"th humble number is "<<dp[number]<<"."<<endl;
    }
}
