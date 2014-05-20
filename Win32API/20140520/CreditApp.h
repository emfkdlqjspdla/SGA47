#pragma once

#pragma comment(lib, "../GameDev/GameDev.lib")

#include "../GameDev/GameDev.h"


class CreditApp : public MainWindow<CreditApp>
{
public :
	CreditApp()
		: lineheight(20)
		, update_dt(0), update_delay(100)
	{
		SetWindowTitle(_T("Credit Test App"));
		SetWindowSize(400, 600);
		SetWindowStyle(WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX);
	}
	~CreditApp()
	{
	}
	void Input(DWORD)
	{
	}
	void Update(DWORD tick)
	{
		if (update_dt > update_delay)
		{
			int count = update_dt / update_delay;

			for (int i = 0; i < count; i++)
			{
				rcText[0] = rcText[0] << Size(0, 10);
				rcText[1] = rcText[1] << Size(0, 10);

				if (rcText[1].bottom < 0)
				{
					rcText[0] = rcClient;
					rcText[0].top = rcClient.bottom;
					rcText[0].bottom = rcText[0].top + lineheight;

					rcText[1] = rcText[0]>>Size(0,lineheight);
				}
			}

			update_dt %= update_delay;
		}

		update_dt += tick;
	}
	void Draw(DWORD)
	{
		backbuffer << RGB(255,255,255);

		::DrawText(backbuffer, _T("만든이"), -1, &rcText[0], DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		::DrawText(backbuffer, _T("Lim J K"), -1, &rcText[1], DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		backbuffer.Draw();
	}

protected :
	void Enter()
	{
		//::strAlloc(szText[0], _T("만든이"));
		//::strAlloc(szText[1], _T("Lim Jong Kyu"));

		rcText[0] = rcClient;
		rcText[0].top = rcClient.bottom;
		rcText[0].bottom = rcText[0].top + lineheight;

		rcText[1] = rcText[0]>>Size(0,lineheight);
	}
	void Leave()
	{
		//for (int i = 0; i < 2; i++)
		//	::SafeDelete(szText[i]);
	}

private :
	//LPTSTR szText[2];
	Rect rcText[2];
	int lineheight;

	DWORD update_dt;
	DWORD update_delay;
};