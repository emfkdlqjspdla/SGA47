#include "BoxManager.h"

BoxManager::BoxManager()
: hOwner(NULL)
{}
BoxManager::~BoxManager()
{}

void BoxManager::Attach(HWND hWnd)
{
	hOwner = hWnd;
}
void BoxManager::Input(DWORD tick)
{
	Rect rc;
	::GetClientRect(hOwner, &rc);

	for (Iter it = boxlist.begin(); it != boxlist.end(); it++)
	{
		Rect tmp;
		if (::IntersectRect(&tmp, &rc, &((*it)->GetDrawRect())))
		{
			(*it)->Input(tick);
		}
	}
}
void BoxManager::Update(DWORD tick)
{
	for (Iter it = boxlist.begin(); it != boxlist.end(); it++)
	{
		(*it)->Input(tick);
	}
}
void BoxManager::Draw(HDC hdc)
{
	Rect rc;
	::GetClientRect(hOwner, &rc);

	for (Iter it = boxlist.begin(); it != boxlist.end(); it++)
	{
		Rect tmp;
		if (::IntersectRect(&tmp, &rc, &((*it)->GetDrawRect())))
		{
			(*it)->Draw(hdc);
		}
	}
}

