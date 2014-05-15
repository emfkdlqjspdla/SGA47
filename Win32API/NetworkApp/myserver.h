#pragma once

#include "server.h"
#include "packet.h"

class myserver : public server
{
public :
	myserver(const char* _szPort = NULL)
		: server(_szPort)
		, recvbuffersize(512)
	{
	}
	~myserver()
	{
	}

	int send(void)
	{
		int iResult = 0;

		iResult = ::send(getSocket(), (const char*)&for_send, *(int*)(unsigned short*)for_send.cbSize, 0);
		if (iResult == SOCKET_ERROR)
		{
			//std::cerr << "send failed : " << ::WSAGetLastError() << std::endl;

			::closesocket(getSocket());
			release();

			return 1;
		}

		//std::clog << "Sent : " << iResult << std::endl;


		return 0;
	}

	int receive(void)
	{
		if (getSocket() == INVALID_SOCKET)
		{
			//std::cerr << "Could not use socket for recv." << std::endl;
			return 2;
		}

		int iResult = 0;
		int recvlen = recvbuffersize;

		vector<clientinfo>::const_iterator it;
		for (it = getClientList().begin(); it != getClientList().end(); it++)
		{
			if (it->getSocket() == INVALID_SOCKET)
				continue;

			iResult = ::recv(it->getSocket(), (char*)&for_recv, recvlen, 0);
			if (iResult > 0)
			{
				//std::clog << "Received : " << iResult << std::endl;
			}
			else if (iResult == 0)
			{
				//std::clog << "Connection closing..." << std::endl;
			}
			else
			{
				//std::clog << "recv failed : " << ::WSAGetLastError() << std::endl;
				::closesocket(it->getSocket());
				
				release();

			}
		}

		return 0;
	}

private :

	packet for_send;
	packet for_recv;

	const int recvbuffersize;
};