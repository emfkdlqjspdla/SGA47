﻿#include "Control.h"

Control::id_type Control::gen_id = 0;

Control::Control()
: hOwner(NULL), cid(gen_id++)
{}
Control::~Control()
{}

void Control::Attach(HWND hWnd)
{
	hOwner = hWnd;
}
void Control::Input(DWORD tick)
{
}
void Control::Update(DWORD tick)
{
}
void Control::Draw(HDC hdc)
{
}

Control::id_type Control::id() const
{
	return cid;
}

