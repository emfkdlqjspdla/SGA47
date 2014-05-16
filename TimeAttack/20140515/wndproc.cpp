#include "windefine.h"

LRESULT CALLBACK WndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	static std::list<Point> pointlist;
	static LONG theta = 0;
	static LONG theta_acc = 1;
	static LONG length = 5;

	if (uMsg == WM_CREATE)
	{
		Rect rc;
		::GetClientRect(hWnd, &rc);

		//Point* pt = new Point(rc.center()>>Size(0,rc.height()/2);

		//pointlist.push_front(pt);

		::SetTimer(hWnd, 0, 50, NULL);
		return 0;
	}
	else if (uMsg == WM_DESTROY)
	{
		::KillTimer(hWnd, 0);

		::PostQuitMessage(0);
		return 0;
	}
	else if (uMsg == WM_PAINT)
	{
		PAINTSTRUCT ps;
		HDC hdc = ::BeginPaint(hWnd, &ps);

		const LONG r = 2;

		std::list<Point>::iterator it;
		for (it = pointlist.begin(); it != pointlist.end(); it++)
		{
			::Rectangle(hdc, it->x - r, it->y - r, it->x + r, it->y + r);
		}

		::EndPaint(hWnd, &ps);
		return 0;
	}
	else if (uMsg == WM_TIMER)
	{
		Rect rc;
		::GetClientRect(hWnd, &rc);

		Point pt;

		LONG x = theta;

		if (x > rc.right)
			x = rc.right;

		pt.x = x+1;
		pt.y = LONG(rc.height()*0.4 * sin(theta*D2R) + rc.height()/2);

		theta += 10;

		pointlist.push_front(pt);

		if (pt.x > rc.right)
		{
			std::list<Point>::iterator it;
			for (it = pointlist.begin(); it != pointlist.end();)
			{
				*it = (*it)<<Size(10,0);

				if (!::PtInRect(&rc, *it))
				{
					it = pointlist.erase(it);
				}
				else
				{
					it++;
				}
			}
		}

		::InvalidateRect(hWnd, &rc, TRUE);

		return 0;
	}

	return ::DefWindowProc(hWnd,uMsg,wParam,lParam);
}