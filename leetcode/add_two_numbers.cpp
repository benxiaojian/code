/*
 * two_sum.c
 *
 *      Author: Nine
 */
#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode(int x) : val(x), next(NULL) {}
 * };
 */


class Solution {
public:
    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
        ListNode *head = NULL;
        ListNode *prev = NULL;
        int count = 0;
        int v1, v2, sum;

        while (l1 || l2) {
            v1 = l1 ? l1->val : 0;
            v2 = l2 ? l2->val : 0;
            sum = v1 + v2 + count;

            count = sum / 10;
            int val = sum % 10;

            ListNode *current = new ListNode(val);
            if (head == NULL) {
                head = current;
            }

            if (prev) {
                prev->next = current;
            }
            prev = current;

            l1 = l1 ? l1->next : NULL;
            l2 = l2 ? l2->next : NULL;

        }

        if (count > 0) {
            prev->next = new ListNode(count);
        }

        return head;
    }
};

