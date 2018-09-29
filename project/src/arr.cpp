#include <iostream>
using namespace std;

void f(int *a)
{
	a[1] = 2;
	//for (int b : a)
	for (int i=0;i<5;i++)
	{
		cout << a[i] << endl;
	}
	cout << endl;
}
int main()
{
	int a[5] = {1,3,5,7,9};
	//for (int i=0;i<5;i++)
	//	cout << a[i] << endl;
	for (int i : a)
		cout << i << endl;
	
		cout << endl;
	f(a);
	for (int i=0;i<5;i++)
		cout << a[i] << endl;
}
