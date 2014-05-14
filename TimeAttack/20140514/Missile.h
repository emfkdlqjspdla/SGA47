#pragma once

#include <windows.h>
#include <tchar.h>
#include "BaseType.h"
#include "Utility.hpp"

class Missile
{
public :
	Missile();
	Missile(const Point& pt, const LONG& r, const LONG& th);
	void Update(DWORD tick);
	void Draw(HDC hdc);

	void SetPosition(const Point& pt);
	void SetTheta(const LONG& th);
	Point GetPosition() const;

private :
	Point center;
	LONG radius;
	LONG theta;
};