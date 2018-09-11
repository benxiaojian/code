#include <iostream>
#include <memory>

using namespace std;

int main()
{
	shared_ptr<int> p(new int(5));
	//p = new int(2);

	cout << "p" << *p << endl;

	//shared_ptr<int> q  = make_shared<int>(p);
	//cout << "q" << *q << endl;
}
