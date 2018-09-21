/*
 * 回文列表
 *
 *      Author: Nine
 */

#include <iostream>
#include <string>
#include <sstream>
#include <cstdio>
#include <cstring>
using namespace std;

// 要求O(n)的时间和O(1)的空间
// 用函数来实现指针的回退


//Definition for singly-linked list.
struct ListNode {
    int val;
    ListNode *next;
    ListNode(int x) : val(x), next(NULL) {}
};

class Solution {
private:
    bool isPal(ListNode *head)
    {
        if (head == NULL) {
            return true;
        }

        if (!isPal(head->next)) {
            return false;
        }

        if (head->val != this->ptr->val) {
            return false;
        }

        this->ptr = this->ptr->next;
        return true;
    }

public:
    bool isPalindrome(ListNode* head) {
        this->ptr = head;

        return isPal(head);
    }
private:
    ListNode *ptr;
};
