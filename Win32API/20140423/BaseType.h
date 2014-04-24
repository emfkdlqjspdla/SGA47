#pragma once

#include <windows.h>

struct Point : public POINT
{
	Point(const LONG& _x = 0, const LONG& _y = 0);
	Point ToScreen(HWND);
	Point ToClient(HWND);
};

struct Size : public SIZE
{
	Size(const LONG& _cx = 0, const LONG& _cy = 0);
};

struct Rect : public RECT
{
	Rect(const LONG& l = 0, const LONG& t = 0, const LONG& r = 0, const LONG& b = 0);
	Rect ToScreen(HWND);
	Rect ToClient(HWND);
};
