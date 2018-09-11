#include <iostream>
#include <stdlib.h>
using namespace std;

typedef int elemType;

typedef struct ListNode{
	elemType data;
	ListNode *next;

}ListNode;

void createList(ListNode **pHead)
{
	(*pHead) = (ListNode *) malloc (sizeof(ListNode));
	if (*pHead)
	{
		(*pHead)->data = 0;
		(*pHead)->next = NULL;
	}
	else 
	{
		cout << "pHead malloc error" << endl;
	}
}

void addNode(ListNode **pHead, elemType pdata)
{
	ListNode *node;
	ListNode *pNode = *pHead;
	node = (ListNode*) malloc (sizeof(ListNode));
	node->data = pdata;
	node->next = NULL;

	while (pNode->next)
	{
		pNode = pNode->next;
	}
	pNode->next = node;
}

void printfList(ListNode *pHead)
{
	while (pHead->next)
	{
		pHead = pHead->next;
		cout << pHead->data << endl;
	}
}
int main()
{
	ListNode *pHead;
	createList(&pHead);
	addNode(&pHead,1);
	addNode(&pHead,2);
	addNode(&pHead,3);
	addNode(&pHead,4);
	addNode(&pHead,5);
	printfList(pHead);
}
