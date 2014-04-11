#include <iostream>
#include <conio.h>
#include "Test.h"

// STL(Standard Template Library)
#include <vector>
#include <list>
#include <string>

using namespace std;

int main(void)
{
	string tmp("hello");

	cout << tmp << endl;
	cout << tmp.length() << endl;
	cout << tmp.size() << endl;

	string::iterator it;
	for (it = tmp.begin(); it != tmp.end(); it++)
	{
		cout << *it << endl;
	}

	tmp = tmp + 'a';
	cout << tmp << endl;
	tmp[0] = 'H';
	cout << tmp << endl;

	string buffer("hello world");

	if (tmp == buffer)
	{
		cout << "same" << endl;
	}

	
	

	_getch();
 	return 0;
}