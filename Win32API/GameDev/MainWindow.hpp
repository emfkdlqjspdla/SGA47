#pragma once

#include <windows.h>
#include <tchar.h>
#include <map>
#include "Callable.hpp"
#include "DoubleBuffer.h"

template<typename T>
class MainWindow
{
	typedef MainWindow<T> Me;
	typedef Callable<LRESULT,T,LRESULT(T::*)(HWND,UINT,WPARAM,LPARAM)> Action;
	typedef typename Action::action_type action_type;
	typedef std::map<UINT,Action> EvtMapType;

public :
	MainWindow()
		: szClassName(_T("MainWindowClass"))
		, width(400), height(400)
		, dwStyle(WS_OVERLAPPEDWINDOW)
		, szWindowTitle(NULL)
		, hMainWnd(NULL)
		, hMainDC(NULL)
	{
	}
	virtual ~MainWindow()
	{
		::SafeDelete(szWindowTitle);
	}
	bool Setup(HINSTANCE hInst)
	{
		WNDCLASSEX wcex;

		wcex.cbSize = sizeof(WNDCLASSEX);
		wcex.style = CS_VREDRAW | CS_HREDRAW;
		wcex.lpfnWndProc = &MainWindow::WndProc;
		wcex.cbClsExtra = 0;
		wcex.cbWndExtra = 0;
		wcex.hInstance = hInst;
		wcex.hIcon = ::LoadIcon(NULL, IDI_APPLICATION);
		wcex.hCursor = ::LoadCursor(NULL, IDC_ARROW);
		wcex.hbrBackground = (HBRUSH)::GetStockObject(WHITE_BRUSH);
		wcex.lpszMenuName = NULL;
		wcex.lpszClassName = szClassName;
		wcex.hIconSm = ::LoadIcon(NULL, IDI_APPLICATION);

		if (!::RegisterClassEx(&wcex))
		{
			DWORD dwError = ::GetLastError();
			return false;
		}

		InitEventMap();

		int cx = ::GetSystemMetrics(SM_CXSCREEN);
		int cy = ::GetSystemMetrics(SM_CYSCREEN);

		int x = (cx - width)/2;
		int y = (cy - height)/2;

		HWND hWnd = ::CreateWindowEx(0, 
					szClassName, 
					((szWindowTitle)? szWindowTitle : _T("Win32 Sample")),
					dwStyle, 
					x, y,
					width, height,
					NULL,
					NULL,
					hInst,
					this);

		if (hWnd == NULL)
		{
			DWORD dwError = ::GetLastError();
			return false;
		}

		::ShowWindow(hWnd, SW_NORMAL);
		::UpdateWindow(hWnd);

		return true;
	}
	INT Run(void)
	{
		MSG msg;

		DWORD dt = 1;
		DWORD st = ::GetTickCount();

		while (true)
		{
			InputDevice.Update(dt);

			if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}
			if (msg.message == WM_QUIT)
				break;


			Input(dt);
			Update(dt);
			Draw(dt);

			dt = ::GetTickCount() - st;
			st = ::GetTickCount();
		}

		return msg.wParam;
	}
	virtual void Input(DWORD tick) = 0;
	virtual void Update(DWORD tick) = 0;
	virtual void Draw(DWORD tick) = 0;

protected :
	virtual void Enter()
	{
	}
	virtual void Leave()
	{
	}
	void SetWindowStyle(DWORD style)
	{
		dwStyle = style;
	}
	void SetWindowTitle(LPCTSTR szTitle)
	{
		::strAlloc(szWindowTitle, szTitle);
	}
	void SetWindowSize(const int& w, const int& h)
	{
		width = w;
		height = h;
	}

protected :
	virtual void InitEventMap()
	{
		SetEventHandler(WM_CREATE, &Me::OnCreate);
		SetEventHandler(WM_DESTROY, &Me::OnDestroy);
	}
	void SetEventHandler(const UINT& uMsg, Action act)
	{
		EvtMapType::iterator it = eventmap.find(uMsg);
		if (it != eventmap.end())
		{
			it->second = act;
			return;
		}

		eventmap.insert(std::make_pair(uMsg, act));
	}
	LRESULT OnCreate(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		backbuffer.Attach(hWnd);
		backbuffer << RGB(255,255,255);

		hMainWnd = hWnd;
		hMainDC = ::GetDC(hWnd);
		::GetClientRect(hWnd, &rcClient);

		Enter();

		return 0;
	}
	LRESULT OnDestroy(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		Leave();

		::ReleaseDC(hWnd, hMainDC);

		backbuffer.Detach();

		::PostQuitMessage(0);
		return 0;
	}
	LRESULT EventHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		EvtMapType::iterator it = eventmap.find(uMsg);
		if (it != eventmap.end())
		{
			return (it->second)(this, hWnd, uMsg, wParam, lParam);
		}

		return ::DefWindowProc(hWnd,uMsg,wParam,lParam);
	}
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		MainWindow* pThis = reinterpret_cast<MainWindow*>(::GetWindowLongPtr(hWnd, GWLP_USERDATA));

		if (pThis)
		{
			return pThis->EventHandler(hWnd, uMsg, wParam, lParam);
		}
		else
		{
			if (uMsg == WM_CREATE)
			{
				CREATESTRUCT* lpcs = reinterpret_cast<CREATESTRUCT*>(lParam);
				pThis = reinterpret_cast<MainWindow*>(lpcs->lpCreateParams);

				::SetWindowLongPtr(hWnd, GWLP_USERDATA, PtrToLong(pThis));

				return pThis->EventHandler(hWnd,uMsg,wParam,lParam);
			}
		}

		return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

private :
	LPCTSTR szClassName;
	LPTSTR szWindowTitle;
	int width;
	int height;
	DWORD dwStyle;
	EvtMapType eventmap;

protected :
	HWND hMainWnd;
	HDC hMainDC;
	Rect rcClient;
	DoubleBuffer backbuffer;
};