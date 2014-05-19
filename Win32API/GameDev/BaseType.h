#pragma once

#include <windows.h>

struct Point;
struct Size;
struct Rect;

struct Point : public POINT
{
	Point(const LONG& _x = 0, const LONG& _y = 0);
	Point ToScreen(HWND);
	Point ToClient(HWND);
	Point operator << (const Size& cs);
	Point operator >> (const Size& cs);
	Point operator - (const Point& o);
};

// wrapper struct
struct Size : public SIZE
{
	Size(const LONG& _cx = 0, const LONG& _cy = 0);

	Size operator - ();

	friend Size operator * (const LONG& v, const Size& obj);
	friend Size operator * (const Size& obj, const LONG& v);
	friend Size operator * (const float& v, const Size& obj);
	friend Size operator * (const Size& obj, const float& v);
};

struct Rect : public RECT
{
	Rect(const LONG& l = 0, const LONG& t = 0, const LONG& r = 0, const LONG& b = 0);
	Rect(const Point& lt, const Point& rb);
	Rect(const Point& ct, const Size& cs);
	Point lefttop() const;
	Point rightbottom() const;
	LONG width() const;
	LONG height() const;
	Point center() const;
	Rect ToScreen(HWND);
	Rect ToClient(HWND);

	Rect Inflate(const Size& diff) const;
	Rect Offset(const Size& diff) const;
	Rect operator << (const Size& diff) const;
	Rect operator >> (const Size& diff) const;
};

struct Color
{
	Color(const COLORREF& _clr = RGB(255,255,255));
	Color(const BYTE& r, const BYTE& g, const BYTE& b);
	Color(const char* szColor);

	int c2d(const char& c);
	COLORREF s2d(const char* s);

	operator COLORREF ();
	Color& operator ++ ();
	Color operator ++ (int);
	Color& operator -- ();
	Color operator -- (int);
	Color operator ! ();
	Color operator - (const Color& o) const;
	Color operator - (const BYTE& byte) const;
	Color operator + (const Color& o) const;
	Color operator + (const BYTE& byte) const;
	bool operator == (const Color& o) const;
	bool operator != (const Color& o) const;
	Color operator / (const int i) const;
	Color operator >> (const Color& o) const;

	COLORREF clr;
	BYTE diff;
};