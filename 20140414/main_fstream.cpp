#include <iostream>
#include <conio.h>
#include <fstream>
#include <string>
#include <stdio.h>

using namespace std;

struct ItemInfo
{
	int price;
	char name[20];

	friend ostream& operator << (ostream& os, const ItemInfo& obj)
	{
		os << obj.name << ", " << obj.price << endl;
		return os;
	}
};

int main(void)
{
	fstream file;

	file.open("test.txt", ios_base::out | ios_base::binary);

	ItemInfo a;
	a.price = 10;
	strcpy_s(a.name, 20, "hello");

	//file << a << endl;

	file.write((const char*)&a, sizeof(ItemInfo));

	file.close();

	file.open("test.txt", ios_base::in | ios_base::binary);

	ItemInfo b;

	file.read((char*)&b, sizeof(ItemInfo));

	file.close();

	cout << b.name << ", " << b.price << endl;

	_getch();
	return 0;
}
