#pragma once

#include <math.h>
#include <windows.h>

class Object
{
public :
	virtual void Update(DWORD tick) = 0;
	virtual void Draw(HDC hdc) = 0;
	POINT getCenter() const
	{
		return ptCenter;
	}
	LONG getRadius() const
	{
		return radius;
	}
	void SetCenter(const POINT& pt)
	{
		ptCenter.x = pt.x;
		ptCenter.y = pt.y;
	}

protected :
	POINT ptCenter;
	LONG radius;
};

class Circle : public Object
{
public :
	Circle(const SIZE& _area)
		: area(_area)
	{
		velocity = 0;

		ptCenter.x = rand()%300 + 50;
		ptCenter.y = rand()%100 + 50;

		radius = rand()%50 + 10;

		BYTE r = rand()%256;
		BYTE g = rand()%256;
		BYTE b = rand()%256;

		color = RGB(r,g,b);
	}
	virtual void Update(DWORD tick)
	{
		velocity += 1;
		ptCenter.y += velocity;

		if (ptCenter.y + radius >= area.cy)
		{
			velocity = -velocity - 1;
		}

		if (ptCenter.x + radius >= area.cx)
		{
			ptCenter.x -= radius;
		}
		else if (ptCenter.x - radius <= 0)
		{
			ptCenter.x += radius;
		}
	}
	virtual void Draw(HDC hdc)
	{
		HPEN hPen = ::CreatePen(PS_SOLID, 1, color);
		HPEN hOldPen = (HPEN)::SelectObject(hdc, hPen);

		HBRUSH hBrush = ::CreateSolidBrush(color);
		HBRUSH hOldBrush = (HBRUSH)::SelectObject(hdc, hBrush);

		::Ellipse(hdc, 
			ptCenter.x - radius,
			ptCenter.y - radius,
			ptCenter.x + radius,
			ptCenter.y + radius);

		::SelectObject(hdc, hOldBrush);
		::DeleteObject(hBrush);

		::SelectObject(hdc, hOldPen);
		::DeleteObject(hPen);
	}

private :
	LONG velocity;
	COLORREF color;
	SIZE area;
};

class MouseCircle : public Object
{
public :
	MouseCircle(HWND hWnd = NULL)
		: hOwner(hWnd)
	{
		ptCenter.x = 0;
		ptCenter.y = 0;

		radius = 50;

		color = RGB(255,0,0);
	}
	void Attach(HWND hWnd)
	{
		hOwner = hWnd;
	}
	void Update(DWORD tick)
	{
		// GetCursorPos 는 screen coord 값을 가져옴.
		::GetCursorPos(&ptCenter);
		// screen coord => client coord
		::ScreenToClient(hOwner, &ptCenter);
	}
	void Draw(HDC hdc)
	{
		HPEN hPen = ::CreatePen(PS_SOLID, 1, color);
		HPEN hOldPen = (HPEN)::SelectObject(hdc, hPen);

		HBRUSH hBrush = ::CreateSolidBrush(color);
		HBRUSH hOldBrush = (HBRUSH)::SelectObject(hdc, hBrush);

		::Ellipse(hdc, 
			ptCenter.x - radius,
			ptCenter.y - radius,
			ptCenter.x + radius,
			ptCenter.y + radius);

		::SelectObject(hdc, hOldBrush);
		::DeleteObject(hBrush);

		::SelectObject(hdc, hOldPen);
		::DeleteObject(hPen);
	}
private :
	COLORREF color;
	HWND hOwner;
};

bool IsCollide(Object* lhs, Object* rhs);
