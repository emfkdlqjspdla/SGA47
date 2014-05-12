﻿#pragma once

#include <windows.h>
#include <tchar.h>
#include "BaseType.h"
#include "InputDevice.h"

enum cursor_id
{
	CURSOR_NORMAL,
	CURSOR_VERT,	// vertical cursor
	CURSOR_HORT,	// horizontal cursor
	CURSOR_MAX,
};

class MainGame
{
public :
	MainGame();
	virtual ~MainGame();

	void Attach(HWND);
	void Input(DWORD tick);
	void Update(DWORD tick);
	void Draw(void);
	void Load();

private :
	void remove();
	void init(void);
	void release(void);

private :
	HWND hOwner;
	HDC hMainDC;
	HDC hMemDC;
	HBITMAP hMemBitmap;
	HBITMAP hOldMemBitmap;

private :
	HBITMAP hSample[CURSOR_MAX];
	HBITMAP hBlock;
	HBITMAP hGray;
	Rect rcSample[CURSOR_MAX];
	int MouseState;
	Point ptMouse;
	bool bClipMouse;
	Size sBitmap;
	Size sGray;
};