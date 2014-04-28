#pragma once

#include <math.h>
#include <windows.h>
#include <tchar.h>

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
	Circle(const SIZE& _area, const int& i)
		: area(_area)
		, update_dt(0), delay(50)
		, dx(5), xVel(5)
		, velocity(0)
		, index(i)
	{

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
		if (update_dt >= delay)
		{
			int count = update_dt/delay;

			for (int i = 0; i < count; i++)
			{
				velocity += 1;
				ptCenter.y += velocity;

				ptCenter.x += dx;

				if (ptCenter.y + radius >= area.cy)
				{
					velocity = -velocity - 1;
				}

				if (ptCenter.x + radius >= area.cx)
				{
					dx = -xVel;
				}
				else if (ptCenter.x - radius <= 0)
				{
					dx = xVel;
				}
			}

			update_dt %= delay;
		}

		update_dt += tick;
	}
	virtual void Draw(HDC hdc)
	{
		HBITMAP hBitmap = (HBITMAP)::LoadImage(NULL, _T("circle_group.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_SHARED);
		HDC hBitmapDC = ::CreateCompatibleDC(hdc);
		HBITMAP hOldBitmap = (HBITMAP)::SelectObject(hBitmapDC, hBitmap);

		BITMAP bm;
		::GetObject(hBitmap, sizeof(BITMAP), &bm);
		int cx = bm.bmWidth/7;
		int cy = bm.bmHeight;

		//::BitBlt(hdc, ptCenter.x - radius, ptCenter.y - radius,
		//	radius*2, radius*2, hBitmapDC, 0, 0, SRCCOPY);

		::GdiTransparentBlt(hdc, ptCenter.x - radius, ptCenter.y - radius,
			radius*2, radius*2, hBitmapDC, index*cx, 0, cx, cy, RGB(255,255,255));

		::SelectObject(hBitmapDC, hOldBitmap);
		::DeleteDC(hBitmapDC);
		::DeleteObject(hBitmap);

		//HPEN hPen = ::CreatePen(PS_SOLID, 1, color);
		//HPEN hOldPen = (HPEN)::SelectObject(hdc, hPen);

		//HBRUSH hBrush = ::CreateSolidBrush(color);
		//HBRUSH hOldBrush = (HBRUSH)::SelectObject(hdc, hBrush);

		//::Ellipse(hdc, 
		//	ptCenter.x - radius,
		//	ptCenter.y - radius,
		//	ptCenter.x + radius,
		//	ptCenter.y + radius);

		//::SelectObject(hdc, hOldBrush);
		//::DeleteObject(hBrush);

		//::SelectObject(hdc, hOldPen);
		//::DeleteObject(hPen);
	}
	void SetDirection(bool bRight = true)
	{
		if (bRight)
			dx = xVel;
		else
			dx = -xVel;
	}
	bool GetDirection() const
	{
		if (dx > 0)
			return true;
		
		return false;
	}

private :
	LONG velocity;
	COLORREF color;
	SIZE area;
	DWORD update_dt;
	DWORD delay;
	LONG dx;
	LONG xVel;
	int index;
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
