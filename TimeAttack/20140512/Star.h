#pragma once

#define _USE_MATH_DEFINES

#include <windows.h>
#include <tchar.h>
#include <math.h>
#include "BaseType.h"

#define D2R (M_PI/180.)

class Star
{
public :
	Star(const Point& pt, const LONG& _speed, const LONG& _th);
	~Star();
	void Update(DWORD tick);
	void Draw(HDC hdc);

	Point GetPosition() const;

private :
	LONG speed;
	LONG theta;
	HBITMAP hStar;
	Point pos;
	Size size;
	Size drawSize;
	DWORD update_dt;
	DWORD delay;
	BYTE alpha;
};