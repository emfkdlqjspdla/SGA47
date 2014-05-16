#pragma once

#pragma comment(lib, "../GameDev/GameDev.lib")

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif // WIN32_LEAN_AND_MEAN

#include <windows.h>

#include "../GameDev/GameDev.h"
#include <process.h>
#include <sstream>
#include <list>
#include "myserver.h"

class NetworkApp : public MainWindow<NetworkApp>
{
	typedef NetworkApp Me;
	typedef MainWindow<NetworkApp> Base;
public :
	NetworkApp()
		: Server(NULL)
	{}
	void Input(DWORD)
	{
		if (InputDevice[VK_LBUTTON])
		{
			Server = new myserver("8887");
			Server->createsocket("8887");
			Server->bind();

			unsigned id;
			HANDLE hThread = (HANDLE)::_beginthreadex(NULL, 0, &Me::proxy, this, 0, &id);
		}
	}
	void Update(DWORD)
	{
	}
	void Draw(DWORD)
	{
		int lineheight = 40;
		Rect box(rcClient.left, rcClient.top, rcClient.right, rcClient.top + lineheight);

		std::wostringstream oss;
		oss << _T("Client Count : ") << clientList.size();
		::DrawText(backbuffer, oss.str().c_str(), -1, &box, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

		box = box >> Size(0, lineheight);

		std::list<clientinfo>::iterator it;
		for (it = clientList.end(); it != clientList.end(); it++)
		{
			std::wostringstream oss;

			oss << _T("ID : " ) << it->getId();

			::DrawText(backbuffer, oss.str().c_str(), -1, &box, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
			box = box >> Size(0, lineheight);
		}

		backbuffer.Draw();
	}

	void Enter()
	{
	}
	void Leave()
	{
	}

	void listen()
	{
		while (Server->listen() == 0)
		{
			if (Server->accept())
			{
				clientList = Server->getClientList();
			}
		}
	}

	static unsigned int __stdcall proxy(void* arg)
	{
		Me* pThis = reinterpret_cast<Me*>(arg);

		pThis->listen();

		return 0;
	}

protected :
	void InitEventMap()
	{
		Base::InitEventMap();

	}
private :
	myserver* Server;
	std::list<clientinfo> clientList;
};
