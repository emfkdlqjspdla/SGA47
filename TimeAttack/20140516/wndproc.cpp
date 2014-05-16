#include "windefine.h"

LRESULT CALLBACK WndProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	static std::list<Box*> boxlist;

	if (uMsg == WM_CREATE)
	{
		Rect rc;
		::GetClientRect(hWnd, &rc);

		Rect rcBox(0,0,100,50);

		Box* pBox = new Box;
		pBox->Attach(hWnd);
		pBox->SetDrawRect(rcBox>>Size(50,50));
		pBox->SetColor(RGB(rand()%100 + 100,rand()%100 + 100,rand()%100 + 100));

		boxlist.push_back(pBox);

		return 0;
	}
	else if (uMsg == WM_DESTROY)
	{
		std::list<Box*>::iterator it;
		for (it = boxlist.begin();
			it != boxlist.end();)			
		{
			delete (*it);
			it = boxlist.erase(it);
		}

		::PostQuitMessage(0);
		return 0;
	}
	else if (uMsg == WM_PAINT)
	{
		Rect rc;
		::GetClientRect(hWnd, &rc);

		PAINTSTRUCT ps;
		HDC hdc = ::BeginPaint(hWnd, &ps);

		int DrawCount = 0;

		std::list<Box*>::iterator it;
		for (it = boxlist.begin();
			it != boxlist.end(); it++)
		{
			Rect tmp;
			if (::IntersectRect(&tmp, &rc, &((*it)->GetDrawRect())))
			{
				(*it)->Draw(hdc);
				DrawCount++;
			}
		}

		std::wostringstream oss;

		oss << _T("Box Count : ") << boxlist.size() << _T("\r\n");
		oss << _T("Draw Count : ") << DrawCount;

		::DrawText(hdc, oss.str().c_str(), -1, &rc, DT_TOP);

		::EndPaint(hWnd, &ps);
		return 0;
	}
	else if (uMsg == WM_LBUTTONDOWN)
	{
		Rect rc;
		::GetClientRect(hWnd, &rc);

		std::list<Box*>::iterator it;
		for (it = boxlist.begin();
			it != boxlist.end(); it++)
		{
			Rect tmp;
			if (::IntersectRect(&tmp, &rc, &((*it)->GetDrawRect())))
			{
				(*it)->Input(0);
				(*it)->Update(0);
			}
		}

		Size Down(0, 70);

		for (it = boxlist.begin();
			it != boxlist.end();)
		{
			if ((*it)->IsSelect())
			{
				Rect tmp = (*it)->GetDrawRect();
				tmp = tmp>>Down;

				Box* pBox = new Box;
				pBox->Attach(hWnd);
				pBox->SetDrawRect(tmp);
				pBox->SetColor(RGB(rand()%100 + 100,rand()%100 + 100,rand()%100 + 100));

				it++;
				boxlist.insert(it, pBox);

				std::list<Box*>::iterator jt;
				for (jt = it; jt != boxlist.end(); jt++)
				{
					(*jt)->SetDrawRect((*jt)->GetDrawRect()>>Down);
				}
			}
			else
			{
				it++;
			}
		}

		::InvalidateRect(hWnd, &rc, TRUE);

		return 0;
	}

	return ::DefWindowProc(hWnd,uMsg,wParam,lParam);
}