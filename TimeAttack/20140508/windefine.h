#pragma once

#include <windows.h>
#include <windowsx.h>
#include <tchar.h>

enum cursor_id
{
	CURSOR_NORMAL,
	CURSOR_VERT,	// vertical cursor
	CURSOR_HORT,	// horizontal cursor
	CURSOR_MAX,
};

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);


/*
20140423 : 0
20140424 : 1
20140425 : 0
20140428 : 0
20140429 : 0
20140430 : 0
20140501 : 0
20140502 : 0
*/