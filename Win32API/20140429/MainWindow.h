#pragma once

#include <windows.h>
#include <tchar.h>
#include <map>
#include "Callable.h"

template<typename T>
class MainWindow
{
	typedef Callable<LRESULT,T,LRESULT(T::*)(HWND,UINT,WPARAM,LPARAM)> Action;
	typedef std::map<UINT,Action> EvtMapType;

public :
	MainWindow()
		: szClassName(_T("MainWindowClass"))
		, width(400), height(400)
		, dwStyle(WS_OVERLAPPEDWINDOW)
	{
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

		int cx = ::GetSystemMetrics(SM_CXSCREEN);
		int cy = ::GetSystemMetrics(SM_CYSCREEN);

		int x = (cx - width)/2;
		int y = (cy - height)/2;


		//DWORD dwStyle = WS_OVERLAPPEDWINDOW;
		//dwStyle = dwStyle & ~WS_MAXIMIZEBOX;
		//dwStyle = dwStyle & ~WS_MINIMIZEBOX;
		///dwStyle = dwStyle & ~WS_THICKFRAME;

		HWND hWnd = ::CreateWindowEx(0, 
					szClassName, 
					_T("Win32 Sample"),
					dwStyle, 
					x,
					y,
					width,
					height,
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
	LRESULT OnCreate(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		return 0;
	}
	LRESULT OnDestroy(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
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
	int width;
	int height;
	DWORD dwStyle;
	EvtMapType eventmap;
};