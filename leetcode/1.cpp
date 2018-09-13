/*
 * two_sum.c
 *
 *      Author: Nine
 */
#include <iostream>
#include <vector>
using namespace std;

// 往map里面放<index, number>， 第一个值A会放到map里面去，然后sum - 另外一个值B， 可以在map里面找到对应的A

class Solution {
public:
    vector<int> twoSum(vector<int>& nums, int target) {
        unordered_map <int, int> map;
        for (int i=0; i<nums.size(); ++i) {
            if (map.count(target - nums[i])) {
                return { map.find(target - nums[i])->second, i };
            } else {
                map.insert(nums[i], i);
            }
        }
    }
};
