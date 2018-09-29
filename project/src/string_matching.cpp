/*
 * string_matching.cpp
 *
 *      Author: Nine
 */

#include <iostream>
#include <map>
using namespace std;


bool isMatching(string &source, string &target)
{
    map<char, char> matching_map;
    map<char, char>::iterator it;

    for (int i = 0; i < target.length(); ++i) {
        it = matching_map.find(target[i]);
        if (it == matching_map.end()) {
            matching_map.insert(make_pair(target[i], source[i]));
        } else {
            if (it->second != source[i]) {
                return false;
            }
        }
    }

    return true;
}


int main()
{
    string A,B;
    int sum = 0;

    cin >> A >> B;

    for (int i = 0;  i <= A.length() - B.length(); ++i) {
        string c = "";
        for (int j = 0; j < B.length(); ++j) {
            c += A[i+j];
        }

        if (isMatching(c, B)) {
            sum++;
        }
    }

    cout << sum << endl;
}
