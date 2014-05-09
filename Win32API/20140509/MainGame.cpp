#include "MainGame.h"

MainGame::MainGame()
: hOwner(NULL)
, hMainDC(NULL)
, hMemDC(NULL)
, hMemBitmap(NULL)
, hOldMemBitmap(NULL)
{
	for (int i = 0; i < CURSOR_MAX; i++)
	{
		hSample[CURSOR_MAX] = NULL;
	}
	hBlock = NULL;
	hGray = NULL;
	MouseState = CURSOR_NORMAL;
	bClipMouse = true;
}
MainGame::~MainGame()
{
	remove();
	release();
}

void MainGame::Attach(HWND hWnd)
{
	release();

	hOwner = hWnd;

	init();
}
void MainGame::Input(DWORD tick)
{
	InputDevice.Update(tick);

	if (bClipMouse)
	{
		ptMouse = InputDevice.pos();
		ptMouse = ptMouse.ToClient(hOwner);
	}

	if (InputDevice[VK_ESCAPE])
	{
		bClipMouse = !bClipMouse;
		if (bClipMouse)
		{
			Rect rc;
			::GetClientRect(hOwner, &rc);

			::ClipCursor(&rc.ToScreen(hOwner));
			::ShowCursor(FALSE);
		}
		else
		{
			::ClipCursor(NULL);
			::ShowCursor(TRUE);
		}
	}
}
void MainGame::Update(DWORD tick)
{
	if (bClipMouse)
	{
		const int margin = 10;
		Rect rc;
		::GetClientRect(hOwner, &rc);

		if (rc.top + margin > ptMouse.y || rc.bottom - margin < ptMouse.y)
			MouseState = CURSOR_VERT;
		else if (rc.left + margin > ptMouse.x || rc.right - margin < ptMouse.x)
			MouseState = CURSOR_HORT;
		else
			MouseState = CURSOR_NORMAL;
	}
}
void MainGame::Draw(void)
{
	Rect rc;
	::GetClientRect(hOwner, &rc);

	::SetDCBrushColor(hMemDC, RGB(1,1,1));
	::FillRect(hMemDC, &rc, (HBRUSH)::GetStockObject(DC_BRUSH));

	HDC hBlockDC = ::CreateCompatibleDC(hMemDC);
	HBITMAP hOldBitmap = (HBITMAP)::SelectObject(hBlockDC, hBlock);

	for (int y = 0; y < rc.height(); y += sBitmap.cy)
	{
		for (int x = 0; x < rc.width(); x += sBitmap.cx)
		{
			::BitBlt(hMemDC, x, y, sBitmap.cx, sBitmap.cy,
				hBlockDC, 0, 0, SRCCOPY);
		}
	}

	::SelectObject(hBlockDC, hOldBitmap);
	::DeleteDC(hBlockDC);

	HDC hBitmapDC = ::CreateCompatibleDC(hMemDC);
	hOldBitmap = (HBITMAP)::SelectObject(hBitmapDC, hSample[MouseState]);

	::GdiTransparentBlt(hMemDC, 
		ptMouse.x - 50, ptMouse.y - 50, 
		100, 100,
		hBitmapDC,
		rcSample[MouseState].left, rcSample[MouseState].top,
		rcSample[MouseState].right - rcSample[MouseState].left,
		rcSample[MouseState].bottom - rcSample[MouseState].top,
		RGB(255,255,255));

	::SelectObject(hBitmapDC, hOldBitmap);
	::DeleteDC(hBitmapDC);

	if (!bClipMouse)
	{
		HDC hGrayDC = ::CreateCompatibleDC(hMemDC);
		hOldBitmap = (HBITMAP)::SelectObject(hGrayDC, hGray);

		BLENDFUNCTION bf;
		bf.BlendFlags = 0;
		bf.BlendOp = AC_SRC_OVER;
		bf.AlphaFormat = 0;
		bf.SourceConstantAlpha = 127;

		::GdiAlphaBlend(hMemDC, 0, 0, rc.width(), rc.height(),
			hGrayDC, 0, 0, sGray.cx, sGray.cy, bf);

		::SelectObject(hGrayDC, hOldBitmap);
		::DeleteDC(hGrayDC);
	}

	::BitBlt(hMainDC, 0, 0, rc.width(), rc.height(), hMemDC, 0, 0, SRCCOPY);

}
void MainGame::Load(void)
{
	Rect rc;
	::GetClientRect(hOwner, &rc);

	hSample[CURSOR_NORMAL] = (HBITMAP)::LoadImage(NULL, _T("sample.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_SHARED);
	hSample[CURSOR_VERT] = (HBITMAP)::LoadImage(NULL, _T("sample.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_SHARED);
	hSample[CURSOR_HORT] = (HBITMAP)::LoadImage(NULL, _T("sample.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_SHARED);

	hBlock = (HBITMAP)::LoadImage(NULL, _T("block.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_SHARED);

	BITMAP bm;
	::GetObject(hBlock, sizeof(BITMAP), &bm);
	sBitmap = Size(bm.bmWidth, bm.bmHeight);

	hGray = (HBITMAP)::LoadImage(NULL, _T("gray.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_SHARED);

	::GetObject(hGray, sizeof(BITMAP), &bm);
	sGray = Size(bm.bmWidth, bm.bmHeight);

	Rect rcTemp(0,0,100,100);
	for (int i = 0; i < CURSOR_MAX; i++)
	{
		rcSample[i] = rcTemp >> (Size(100, 0)*i);
	}

	ptMouse = Point(rc.width()/2, rc.height()/2);

	::SetCursorPos(ptMouse.ToScreen(hOwner).x, ptMouse.ToScreen(hOwner).y);
	::ShowCursor(FALSE);
}
void MainGame::remove(void)
{
	::ShowCursor(TRUE);

	::DeleteObject(hGray);

	::DeleteObject(hBlock);

	for (int i = 0; i < CURSOR_MAX; i++)
	{
		::DeleteObject(hSample[i]);
	}
}
void MainGame::init(void)
{
	if (!hOwner) return;

	Rect rc;
	::GetClientRect(hOwner, &rc);

	hMainDC = ::GetDC(hOwner);
	hMemDC = ::CreateCompatibleDC(hMainDC);
	hMemBitmap = ::CreateCompatibleBitmap(hMainDC, rc.width(), rc.height());
	hOldMemBitmap = (HBITMAP)::SelectObject(hMemDC, hMemBitmap);

}
void MainGame::release(void)
{
	if (hMemDC)
		::SelectObject(hMemDC, hOldMemBitmap);

	if (hMemBitmap)
		::DeleteObject(hMemBitmap);

	if (hMemDC)
		::DeleteDC(hMemDC);

	if (hMainDC)
		::ReleaseDC(hOwner, hMainDC);
}
