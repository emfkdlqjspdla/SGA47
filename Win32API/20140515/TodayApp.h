#pragma once

#pragma comment(lib, "../GameDev/GameDev.lib")

#include "../GameDev/GameDev.h"
#include "Image.h"
#include "Animation.h"
#include <list>
#include <sstream>

class TodayApp : public MainWindow<TodayApp>
{
public :
	TodayApp()
		: update_dt(0), update_delay(100)
		, imgIndex(0)
	{
	}
	void Input(DWORD)
	{
		ptMouse = InputDevice.pos();
		ptMouse = ptMouse.ToClient(hMainWnd);

		if (InputDevice[VK_LBUTTON])
		{
			Animation* pAni = new Animation;

			for (int i = 0; i < 25; i++)
			{
				Image* pImage = new Image;
				pImage->Load(_T("expl.bmp"), Rect(0 + 64*(i%5),0 + 64*(i/5),64 + 64*(i%5), 64 + 64*(i/5)));
				pImage->SetTransparentColor(RGB(255,0,255));

				pAni->AddImage(pImage);
			}

			pAni->SetDrawRect(Rect(ptMouse, Size(64,64)));

			aniFire.push_back(pAni);
		}

	}
	void Update(DWORD tick)
	{
		std::list<Animation*>::iterator it;
		for (it = aniFire.begin(); it != aniFire.end();)
		{
			(*it)->Update(tick);
			if (!(*it)->IsPlaying())
			{
				delete (*it);
				it = aniFire.erase(it);
			}
			else
			{
				it++;
			}
		}
	}
	void Draw(DWORD)
	{
		backbuffer << RGB(1,255,255);

		std::list<Animation*>::iterator it;
		for (it = aniFire.begin(); it != aniFire.end(); it++)
		{
			(*it)->Draw(backbuffer);
		}

		::DrawText(backbuffer, _T("Hello World"), -1, &rcClient, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

		std::wostringstream oss;

		oss << _T("Animation Count : ") << aniFire.size();

		::DrawText(backbuffer, oss.str().c_str(), -1, &rcClient, DT_LEFT | DT_TOP);

		backbuffer.Draw();
	}

protected :
	void Enter()
	{
	}
	void Leave()
	{
	}

private :
	std::list<Animation*> aniFire;

	DWORD update_dt;
	DWORD update_delay;
	int imgIndex;

	Point ptMouse;
};
