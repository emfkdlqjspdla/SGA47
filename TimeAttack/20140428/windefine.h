#pragma once

#include <windows.h>
#include <windowsx.h>
#include <tchar.h>

enum state_id
{
	NONE,
	OOO,
	XXX,
};


LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);

void check(HDC hdc, const RECT& r, bool);

/*
20140423 : 0
20140424 : 1
20140425 : 0
20140428 : 
*/