#include <iostream>
using namespace std;

void quickSort(int *a,int l,int r)
{
	if (l < r)
	{
		int i = l;
		int j = r;
		int temp = a[l];

		while (i<j)
		{
			while (i<j && a[j]>= temp)
				j--;
			if (i < j)
			{
				a[i++] = a[j];
			}

			while (i<j && a[i]<=temp)
				i++;
			if (i<j)
				a[j--] = a[i];
		}

		a[i] = temp;
		quickSort(a,l,i-1);
		quickSort(a,i+1,r);
	}
}

int main()
{
	int a[5] = {5,2,3,4,1};
	quickSort(a,0,4);
	for (int i=0;i<5;i++)
		cout << a[i] << " ";
	cout << endl;
}
