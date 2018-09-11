#include <iostream>
using namespace std;


void mergeSort(int *a, int begin, int t, int end)
{
	int i = begin;
	int j = t+1;
	int n = t;
	int m = end;
	int *p = new int[end-begin+1];
	int k = 0;

	while (i<=n && j <= end)
	{
		if (a[i] < a[j])
			p[k++] = a[i++];
		else 
			p[k++] = a[j++];

	}
	while (i <= t)
		p[k++] = a[i++];
	while (j <= end) 
		p[k++] = a[j++];
	k = 0;
	while (begin <= end)
	{
		a[begin++] = p[k++];
	}
}

void merge(int *a,int begin,int end)
{
	if (begin < end)
	{
		int t = (begin+end)/2;
		merge(a,begin,t);
		merge(a,t+1,end);
		mergeSort(a,begin,t,end);
	}
}

int main()
{
	int a[5] = {5,3,2,1,4};
	merge(a,0,4);
	for (int i=0;i<5;i++)
		cout << a[i] << " " ;
		cout << endl;
}
