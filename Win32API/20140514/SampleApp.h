#pragma once

#pragma comment(lib, "../GameDev/GameDev.lib")

#include "../GameDev/GameDev.h"
#include <strsafe.h>
#include <sstream>
#include <list>

class SampleApp : public MainWindow<SampleApp>
{
	typedef SampleApp Me;
	typedef MainWindow<SampleApp> Base;
public :
	SampleApp()
		: dt(0), bGrabWindow(false)
		, update_dt(0), update_delay(100), theta(0)
		, bgcolor("4169E1")
	{
		SetWindowStyle(WS_POPUP | WS_VISIBLE);
	}
	void Input(DWORD tick)
	{
		if (InputDevice[VK_LBUTTON])
		{
			bGrabWindow = true;
		}
		else
		{
			bGrabWindow = false;
		}

		if (InputDevice.OnClick(VK_RETURN))
		{
			if (!InputDevice[VK_SHIFT])
				bgcolor++;
			else
				bgcolor--;
		}

		ptPrev = ptMouse;
		ptMouse = InputDevice.pos();
	}
	void Update(DWORD tick)
	{
		dt += tick;

		if (bGrabWindow)
		{
			Point diff = ptMouse - ptPrev;

			Rect rcWin;
			::GetWindowRect(hMainWnd, &rcWin);

			::MoveWindow(hMainWnd, rcWin.left + diff.x, rcWin.top + diff.y, rcWin.width(), rcWin.height(), TRUE);
		}

		if (update_dt > update_delay)
		{
			int count = update_dt/update_delay;

			for (int i = 0; i < count; i++)
			{
				theta += 1;
				if (theta >= 360)
					theta = 0;

				ptEnd.x = LONG(rcClient.center().x + (rcClient.width()/2)*cos((90-theta)*D2R));
				ptEnd.y = LONG(rcClient.center().y - (rcClient.width()/2)*sin((90-theta)*D2R));
			}

			update_dt %= update_delay;
		}

		update_dt += tick;
	}
	void Draw(DWORD tick)
	{
		backbuffer << bgcolor;

		Rect rc;
		::GetClientRect(hMainWnd, &rc);

		std::wostringstream oss;
		oss << _T("Hello World") << dt << _T("\r\n");

		InputProcessor::KeyList::iterator it;
		InputProcessor::KeyList pressedKey = InputDevice.current();
		for (it = pressedKey.begin(); it != pressedKey.end(); it++)
		{
			oss << std::hex << (*it) << _T("\r\n");
		}

		::SetBkMode(backbuffer, TRANSPARENT);

		HPEN hPen = ::CreatePen(PS_SOLID, 15, RGB(255,0,0));
		HPEN hOldPen = ::Select(backbuffer, hPen);

		::SetArcDirection(backbuffer, AD_CLOCKWISE);
		Rect rcArc = rc.Inflate(-Size(15/2,15/2));
		//::Arc(backbuffer, rcArc.left, rcArc.top, rcArc.right, rcArc.bottom, ptStart.x, ptStart.y, ptEnd.x, ptEnd.y);

		::ArcTo(backbuffer, rcArc.left, rcArc.top, rcArc.right, rcArc.bottom, ptStart.x, ptStart.y, ptEnd.x, ptEnd.y);

		::Select(backbuffer, hOldPen);
		::DeleteObject(hPen);

		::DrawText(backbuffer, oss.str().c_str(), -1, &rc, DT_CENTER | DT_TOP);

		backbuffer.Draw();
	}

protected :
	void InitEventMap()
	{
		Base::InitEventMap();

		// TODO
	}
	void Enter()
	{
		Rect rc;
		::GetClientRect(hMainWnd, &rc);

		const LONG size = 50;

		hValidRgn = ::CreateEllipticRgn(rc.left, rc.top, rc.right, rc.bottom);

		::CombineRgn(hValidRgn, hValidRgn, ::CreateRectRgn(rc.right - size, rc.top, rc.right, rc.top + size), RGN_OR);
		::SetWindowRgn(hMainWnd, hValidRgn, TRUE);

		::SetWindowLongPtr(hMainWnd, GWL_EXSTYLE, WS_EX_LAYERED | ::GetWindowLongPtr(hMainWnd, GWL_EXSTYLE));
		::SetLayeredWindowAttributes(hMainWnd, RGB(255,255,255), 200, LWA_ALPHA);

		ptStart = Point(rc.width()/2, 0);
		ptEnd = ptStart;

	}
	void Leave()
	{
		::DeleteObject(hValidRgn);
	}

private :
	DWORD dt;
	Point ptMouse;
	Point ptPrev;
	bool bGrabWindow;

	HRGN hValidRgn;

	Point ptStart;
	Point ptEnd;
	LONG theta;

	DWORD update_dt;
	DWORD update_delay;

	Color bgcolor;
};