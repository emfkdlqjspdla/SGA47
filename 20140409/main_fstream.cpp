#include <iostream>
#include <conio.h>
#include <fstream>
#include <string>

using namespace std;

int main(void)
{
	fstream file;

	file.open("test.txt", ios_base::in);

	string buffer;
	//char buffer[100];

	getline(file, buffer);
	//file >> buffer;

	cout << buffer << endl;

	file.close();

	_getch();
	return 0;
}
