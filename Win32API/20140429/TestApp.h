#pragma once
#include "mainwindow.h"

class TestApp :
	public MainWindow<TestApp>
{
public:
	TestApp(void);
	~TestApp(void);
	LRESULT OnMouseMove(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		// todo

		return 0;
	}

private :
	POINT ptMouse;
};
