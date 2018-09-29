#include <stdio.h>
#include <stdlib.h>
#define ERROR 0
#define OK 1

typedef int ElemData;

typedef struct Node
{
	ElemData data;
	struct Node *next;
}Node;

typedef Node *ListNode;

ListNode listHead;//with head

// create list head node
void createList(ListNode *listHead)
{
	*listHead = (ListNode) malloc (sizeof(Node));
	if (!(listHead)) 
		printf("malloc failed\n");
	(*listHead)->data = 0;
	(*listHead)->next = NULL;
}

//add node into list tail
void addTail(ListNode *listHead,ElemData data)
{
	ListNode p = *listHead;
	ListNode s;
	s = (ListNode) malloc (sizeof (Node));
	if (!s)
		printf("malloc failed\n");
	s->data = data;
	s->next = NULL;
	
	while (p->next)
	{
		p = p->next;
	}
	
	p->next = s;
	(*listHead)->data++;
}

//add node into first
void addFirst(ListNode *listHead,ElemData data)
{
	ListNode p = *listHead;
	ListNode s;
	s = (ListNode) malloc (sizeof (Node));
	if (!s)
		printf("malloc failed\n");
	s->data = data;
	s->next = p->next;
	p->next = s;
	(*listHead)->data++;
}

//insert node at place i
void insert(ListNode *listHead,ElemData data, int place)
{
	ListNode p = *listHead;
	ListNode s;
	if (place < 1 && place > p->data+1)
	{
		printf("place is error\n");
	}	
	
	s = (ListNode) malloc (sizeof(Node));
	if (!s)
		printf("malloc failed\n");
	
	int i = 0;
	while(p->next)
	{
		i++;
		if (i==place) break;
		p = p->next;
	}
	
	s->data = data;
	s->next = p->next;
	p->next = s;
	
	(*listHead)->data++;
}

//print list
void printList(ListNode listHead)
{
	
	if (listHead->data == 0)
	{
		printf("list is empty\n");
		return;
	}
	printf("length:%d\t",listHead->data);
	while (listHead->next)
	{
		listHead = listHead->next;
		printf("%d ",listHead->data);
	}
	printf("\n");
}

//clear list
void clearList(ListNode *listHead)
{
	ListNode p = (*listHead)->next;
	ListNode q ;
	while (p)
	{
		q = p;
		p = p->next;
		free(q);
	}
	(*listHead)->data = 0;
	(*listHead)->next = NULL;
}

//remove node i
void remove1(ListNode *listHead,int place)
{
	ListNode p = *listHead;
	ListNode q;
	if (place <1 &&  place > (*listHead)->data)
		printf("place is error\n");
	int i=0;
	while (p->next)
	{
		q=p;
		p = p->next;
		i++;
		if (i==place)
		{
			q->next = p->next;
			//if (q->next) printf("%d\n",q->next->data);
			free(p);
			break;
		}
	}
	(*listHead)->data--;
}


int main()
{
	createList(&listHead);
	addFirst(&listHead,1);

	addFirst(&listHead,2);

	addTail(&listHead,3);
	
	insert(&listHead,4,2);
	
	insert(&listHead,5,5);
	
	
	printList(listHead);
	
	remove1(&listHead,4);
	printList(listHead);
	
	remove1(&listHead,4);
	printList(listHead);
	
	remove1(&listHead,1);
	printList(listHead);
	
	clearList(&listHead);
	printList(listHead);
}
