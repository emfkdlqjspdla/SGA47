#include <iostream>
#include <conio.h>
#include <fstream>
#include <string>

using namespace std;

int main(void)
{
	fstream file;

	file.open("D:\\background.html", ios_base::in);

	string buffer;

	while (file.good())
	{
		getline(file, buffer);
		cout << buffer << endl;
	}

	file.close();

	_getch();
	return 0;
}
