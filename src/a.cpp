#include <iostream>
#include <stdlib.h>
using namespace std;

class a{
public:
	~a()
	{
		cout << "~a()" << endl;
	}
};

void f()
{
	a b;
}


int main()
{
	f();
	cout << "end main" << endl;
}
