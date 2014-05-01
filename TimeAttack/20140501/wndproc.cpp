#include "windefine.h"

struct Circle
{
	POINT center;
	LONG radius;
	Circle(const LONG& r):radius(r){}
	void SetCenter(const POINT& c)
	{
		center = c;
	}
	void Draw(HDC hdc)
	{
		::Ellipse(hdc, center.x - radius, center.y - radius,
			center.x + radius, center.y + radius);
	}
	POINT follow(const POINT& pt)
	{
		POINT tmp = center;

		center = pt;

		return tmp;
	}
};


LRESULT CALLBACK WndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	const int count = 15;
	static Circle* marble[count];

	if (uMsg == WM_CREATE)
	{
		const int r[count] = {50,40,30,20,10};

		for (int i = 0; i < count; i++)
		{
			marble[i] = new Circle(50 - i*2);
		}

		::SetTimer(hWnd, 0, 50, NULL);
		return 0;
	}
	else if (uMsg == WM_DESTROY)
	{
		for (int i = 0; i < count; i++)
		{
			delete marble[i];
		}

		::KillTimer(hWnd, 0);
		::PostQuitMessage(0);
		return 0;
	}
	else if (uMsg == WM_PAINT)
	{
		PAINTSTRUCT ps;
		HDC hdc = ::BeginPaint(hWnd, &ps);

		for (int i = 0; i < count; i++)
		{
			marble[count-i-1]->Draw(hdc);
		}

		::EndPaint(hWnd, &ps);
		return 0;
	}
	else if (uMsg == WM_MOUSEMOVE)
	{
		POINT ptMouse;
		::GetCursorPos(&ptMouse);
		::ScreenToClient(hWnd, &ptMouse);

		marble[0]->SetCenter(ptMouse);

		//POINT tmp = marble[0]->follow(ptMouse);
		//for (int i = 0; i < count-1; i++)
		//{
		//	tmp = marble[i+1]->follow(tmp);
		//}

		RECT rc;
		::GetClientRect(hWnd, &rc);
		::InvalidateRect(hWnd, &rc, TRUE);

		return 0;
	}
	else if (uMsg == WM_TIMER)
	{
		POINT ptMouse;
		::GetCursorPos(&ptMouse);
		::ScreenToClient(hWnd, &ptMouse);

		POINT tmp = marble[0]->follow(ptMouse);
		for (int i = 0; i < count-1; i++)
		{
			tmp = marble[i+1]->follow(tmp);
		}

		RECT rc;
		::GetClientRect(hWnd, &rc);
		::InvalidateRect(hWnd, &rc, TRUE);

		return 0;
	}

	return ::DefWindowProc(hWnd,uMsg,wParam,lParam);
}