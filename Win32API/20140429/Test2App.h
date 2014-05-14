#pragma once

#pragma comment(lib, "../GameDev/GameDev.lib")

#include "../GameDev/GameDev.h"

class Test2App : public MainWindow<Test2App>
{
	typedef Test2App Me;
	typedef MainWindow<Test2App> Base;
public:
	Test2App(void);
	~Test2App(void);

public :
	virtual void Input(DWORD tick);
	virtual void Update(DWORD tick);
	virtual void Draw(DWORD tick);
protected :
	void InitEventMap()
	{
		Base::InitEventMap();

		SetEventHandler(WM_MOUSEMOVE, &Me::OnMouseMove);
		SetEventHandler(WM_PAINT, &Me::OnPaint);
	}
	LRESULT OnMouseMove(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		// todo
		::GetCursorPos(&ptMouse);
		::ScreenToClient(hWnd, &ptMouse);

		RECT rc;
		::GetClientRect(hWnd, &rc);
		::InvalidateRect(hWnd, &rc, TRUE);

		return 0;
	}
	LRESULT OnPaint(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		PAINTSTRUCT ps;
		HDC hdc = ::BeginPaint(hWnd, &ps);

		::SetDCBrushColor(hdc, RGB(255,200,100));
		HBRUSH hOldBrush = (HBRUSH)::SelectObject(hdc, (HBRUSH)::GetStockObject(DC_BRUSH));

		::Ellipse(hdc, ptMouse.x - 20, ptMouse.y - 20, ptMouse.x + 20, ptMouse.y + 20);

		::SelectObject(hdc, hOldBrush);

		::EndPaint(hWnd, &ps);
		return 0;
	}

private :
	POINT ptMouse;
};
