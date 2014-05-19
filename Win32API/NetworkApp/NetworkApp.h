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
#include "logger.h"

class NetworkApp : public MainWindow<NetworkApp>
{
	typedef NetworkApp Me;
	typedef MainWindow<NetworkApp> Base;
public :
	NetworkApp()
		: Server(NULL)
	{
		Log << "NetworkApp Start." << std::endl;
	}
	~NetworkApp()
	{
		Log << "NetworkApp End." << std::endl;
	}
	void Input(DWORD)
	{
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

		std::list<clientinfo*>::iterator it;
		for (it = clientList.end(); it != clientList.end(); it++)
		{
			std::wostringstream oss;

			oss << _T("ID : " ) << (*it)->getId();

			::DrawText(backbuffer, oss.str().c_str(), -1, &box, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
			box = box >> Size(0, lineheight);
		}

		backbuffer.Draw();
	}

	void Enter()
	{
		Log << "Enter in." << std::endl;

		::InitializeCriticalSection(&cs);

		Log << "thread before." << std::endl;

		unsigned id;
		HANDLE hThread = (HANDLE)::_beginthreadex(NULL, 0, &Me::proxy, this, 0, &id);

		Log << "thread after." << std::endl;
	}
	void Leave()
	{
		::DeleteCriticalSection(&cs);
	}

	void listen()
	{
		Log << "listen Start." << std::endl;

		Server = new myserver("8887");
		Server->seterrbuf(logger::getReference().rdbuf());
		Log << "Server allocated : " << Server << std::endl;

		Server->Run();


		while (Server->listen() == 0)
		{
			Log << "while in." << std::endl;

			clientinfo* pInfo = NULL;
			if (pInfo = Server->accept())
			{
				Log << "accepted." << std::endl;
				Server->receive();
				Server->send();
			}

			::EnterCriticalSection(&cs);

			clientList = Server->getClientList();

			::LeaveCriticalSection(&cs);

			Log << "while out." << std::endl;
		}
	}

	static unsigned int __stdcall proxy(void* arg)
	{
		Log << "proxy Start." << std::endl;

		Me* pThis = reinterpret_cast<Me*>(arg);

		pThis->listen();

		return 0;
	}

protected :
	//void InitEventMap()
	//{
	//	Base::InitEventMap();

	//}
private :
	myserver* Server;
	std::list<clientinfo*> clientList;

	CRITICAL_SECTION cs;
};
