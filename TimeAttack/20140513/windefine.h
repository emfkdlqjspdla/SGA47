#pragma once

#include <windows.h>
#include <windowsx.h>
#include <tchar.h>
#include <list>
#include "BaseType.h"
#include "Utility.hpp"
#include "Image.h"

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);

void DrawCard(HDC hdc, std::list<Image*>& depot, const Rect& rc, const int& current, const int& center, const BYTE& alpha = 255);
/*
20140423 : 0
20140424 : 1
20140425 : 0
20140428 : 0
20140429 : 0
20140430 : 0
20140501 : 0
20140502 : 0
20140508 : 3
*/