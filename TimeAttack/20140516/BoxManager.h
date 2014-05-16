#pragma once

#include <list>
#include "Box.h"

class BoxManager
{
	typedef std::list<Box*> List;
	typedef List::iterator Iter;
public :
	BoxManager();
	~BoxManager();

	void Attach(HWND);
	void Input(DWORD);
	void Update(DWORD);
	void Draw(HDC);

private :
	HWND hOwner;

	List boxlist;
};