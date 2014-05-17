﻿#include "BaseType.h"

Point::Point(const LONG& _x/*=0*/, const LONG& _y/*=0*/)
{
	x = _x;
	y = _y;
}
Point Point::ToScreen(HWND hWnd)
{
	Point tmp(*this);

	::ClientToScreen(hWnd, &tmp);

	return tmp;
}
Point Point::ToClient(HWND hWnd)
{
	Point tmp(*this);

	::ScreenToClient(hWnd, &tmp);

	return tmp;
}
Point Point::operator << (const Size& cs)
{
	return Point(x - cs.cx, y - cs.cy);
}
Point Point::operator >> (const Size& cs)
{
	return Point(x + cs.cx, y + cs.cy);
}

Size::Size(const LONG& _cx/*=0*/, const LONG& _cy/*=0*/)
{
	cx = _cx;
	cy = _cy;
}
Size operator * (const LONG& v, const Size& obj)
{
	return Size(obj.cx*v, obj.cy*v);
}
Size operator * (const Size& obj, const LONG& v)
{
	return Size(obj.cx*v, obj.cy*v);
}
Size operator * (const float& v, const Size& obj)
{
	return Size(LONG(obj.cx*v), LONG(obj.cy*v));
}
Size operator * (const Size& obj, const float& v)
{
	return Size(LONG(obj.cx*v), LONG(obj.cy*v));
}

Rect::Rect(const LONG& l/*=0*/, const LONG& t/*=0*/, const LONG& r/*=0*/, const LONG& b/*=0*/)
{
	left = l;
	top = t;
	right = r;
	bottom = b;
}
Rect::Rect(const Point& lt, const Point& rb)
{
	left = lt.x;
	top = lt.y;
	right = rb.x;
	bottom = rb.y;
}
Rect::Rect(const Point& ct, const Size& cs)
{
	left = ct.x - cs.cx/2;
	top = ct.y - cs.cy/2;
	right = left + cs.cx;
	bottom = top + cs.cy;
}
Point Rect::lefttop() const
{
	return Point(left, top);
}
Point Rect::rightbottom() const
{
	return Point(right, bottom);
}
LONG Rect::width() const
{
	return (right - left);
}
LONG Rect::height() const
{
	return (bottom - top);
}
Point Rect::center() const
{
	return Point((left+right)/2, (top+bottom)/2);
}
Rect Rect::ToScreen(HWND hWnd)
{
	Point lt = lefttop();
	Point rb = rightbottom();

	return Rect(lt.ToScreen(hWnd), rb.ToScreen(hWnd));
}
Rect Rect::ToClient(HWND hWnd)
{
	Point lt = lefttop();
	Point rb = rightbottom();

	return Rect(lt.ToClient(hWnd), rb.ToClient(hWnd));
}
Rect Rect::Offset(const Size& diff) const
{
	return Rect(left + diff.cx, top + diff.cy, right + diff.cx, bottom + diff.cy);
}
Rect Rect::operator << (const Size& diff) const
{
	return Rect(left - diff.cx, top - diff.cy, right - diff.cx, bottom - diff.cy);
}
Rect Rect::operator >> (const Size& diff) const
{
	return Rect(left + diff.cx, top + diff.cy, right + diff.cx, bottom + diff.cy);
}


