#pragma once

#include <windows.h>
#include <tchar.h>
#include "BaseType.h"

class Star
{
public :
	Star(const LONG& _speed);
	~Star();
	void Update(DWORD tick);
	void Draw(HDC hdc);

private :
	LONG speed;
	HBITMAP hStar;
	Point pos;
	Size size;
	Size drawSize;
	DWORD update_dt;
	DWORD delay;
};