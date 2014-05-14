#pragma once

#include <windows.h>
#include <tchar.h>
#include "BaseType.h"
#include "Utility.hpp"
#include "InputDevice.h"
#include "Missile.h"

#include <list>

class Tank
{
	typedef std::list<Missile*> MissileList;
public :
	Tank();
	Tank(const Point& pt, const LONG& r);
	~Tank();
	void Input(DWORD tick);
	void Update(DWORD tick);
	void Draw(HDC hdc);

	void SetPosition(const Point& pt);

private :
	Point center;
	LONG radius;
	LONG length;
	Point last;
	LONG theta;

	MissileList MissileDepot;

	DWORD update_dt;
	DWORD update_delay;

	DWORD input_dt;
	DWORD input_delay;
};