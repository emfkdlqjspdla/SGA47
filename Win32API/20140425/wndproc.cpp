#include "windefine.h"

LRESULT CALLBACK WndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	static const int count = 10;
	static Object* marble[count] = {0};
	static Object* objMouse = NULL;
	static DWORD dt = 1;
	static DWORD st = ::GetTickCount();
	static RECT rc;

	if (uMsg == WM_CREATE)
	{
		::GetClientRect(hWnd, &rc);
		SIZE area = {rc.right-rc.left, rc.bottom-rc.top};

		// 원을 만들기
		for (int i = 0; i < count; i++)
		{
			marble[i] = new Circle(area, rand()%7);
		}

		objMouse = new MouseCircle(hWnd);

		::SetTimer(hWnd, 0, 10, NULL);
		return 0;
	}
	else if (uMsg == WM_DESTROY)
	{
		// 만든 원을 삭제..
		for (int i = 0; i < count; i++)
		{
			delete marble[i];
		}

		delete objMouse;

		::KillTimer(hWnd, 0);
		::PostQuitMessage(0);
		return 0;
	}
	else if (uMsg == WM_PAINT)
	{
		int cx = rc.right - rc.left;
		int cy = rc.bottom - rc.top;

		PAINTSTRUCT ps;
		// Handle Device Context
		HDC hdc = ::BeginPaint(hWnd, &ps);

		// DoubleBuffer 를 위한 DC 생성/Bitmap 생성
		HDC hMemDC = ::CreateCompatibleDC(hdc);
		HBITMAP hMemBitmap = ::CreateCompatibleBitmap(hdc, cx, cy);
		HBITMAP hOldMemBitmap = (HBITMAP)::SelectObject(hMemDC, hMemBitmap);

		// 생성된 Bitmap은 검정색이라서 다른색으로 채워준다.
		::SetDCBrushColor(hMemDC, RGB(0,255,255));
		::FillRect(hMemDC, &rc, (HBRUSH)::GetStockObject(DC_BRUSH));

		// 만든 원 그리기..
		for (int i = 0; i < count; i++)
		{
			marble[i]->Draw(hMemDC);
		}

		objMouse->Draw(hMemDC);

		// 메모리DC의 내용을 실제 화면에 뿌려준다.
		::BitBlt(hdc, 0, 0, cx, cy, hMemDC, 0, 0, SRCCOPY);

		// 메모리DC/메모리Bitmap을 삭제.
		::SelectObject(hMemDC, hOldMemBitmap);
		::DeleteObject(hMemBitmap);
		::DeleteDC(hMemDC);

		::EndPaint(hWnd, &ps);
		return 0;
	}
	else if (uMsg == WM_ERASEBKGND)
	{
		return 1;
	}
	else if (uMsg == WM_TIMER)
	{


		// 만든 원 업데이트.
		for (int i = 0; i < count; i++)
		{
			// 충돌을 했다면...
			if (IsCollide(marble[i], objMouse))
			{
				POINT pt = marble[i]->getCenter();
				if (marble[i]->getCenter().x > objMouse->getCenter().x)
				{
					pt.x += objMouse->getRadius();
				}
				else
				{
					pt.x -= objMouse->getRadius();
				}
				Circle* pCircle = dynamic_cast<Circle*>(marble[i]);
				pCircle->SetDirection(!pCircle->GetDirection());
				marble[i]->SetCenter(pt);
			}

			//for (int j = i+1; j < count; j++)
			//{
			//	if (IsCollide(marble[i], marble[j]))
			//	{
			//		POINT pt = marble[i]->getCenter();
			//		if (marble[i]->getCenter().x > marble[j]->getCenter().x)
			//		{
			//			pt.x += marble[j]->getRadius();
			//		}
			//		else
			//		{
			//			pt.x -= marble[j]->getRadius();
			//		}
			//		marble[i]->SetCenter(pt);
			//	}
			//}

			marble[i]->Update(dt);
		}

		objMouse->Update(dt);

		dt = ::GetTickCount() - st;
		st = ::GetTickCount();

		RECT rc;
		::GetClientRect(hWnd, &rc);
		::InvalidateRect(hWnd, &rc, TRUE);
	}

	return ::DefWindowProc(hWnd,uMsg,wParam,lParam);
}
