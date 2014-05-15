#pragma once

#include "network.h"
#include <list>

using std::list;

struct clientinfo
{
	typedef clientinfo _Myt;

	clientinfo()
		: id(gen_id++), Socket(INVALID_SOCKET)
	{
	}
	clientinfo(const _Myt& o)
		: id(o.id), Socket(o.Socket)
	{
	}
	~clientinfo()
	{
	}
	_Myt& operator = (const _Myt& o)
	{
		id = o.id;
		Socket = o.Socket;

		return (*this);
	}

	inline const SOCKET& getSocket(void) const
	{
		return Socket;
	}
	inline void setSocket(const SOCKET& _Socket)
	{
		Socket = _Socket;
	}
	inline const unsigned int& getId(void) const
	{
		return id;
	}
	inline addrinfo& getAIRef(void)
	{
		return ai;
	}

private :
	unsigned int id;
	SOCKET Socket;
	addrinfo ai;

	static unsigned int gen_id;
};

unsigned int clientinfo::gen_id = 0;

class server : public network
{
public :
	server(const char* _szPort = NULL)
	{
		setport(_szPort);
	}
	~server()
	{
	}

	int createsocket(const char* _szPort = NULL)
	{
		setport(_szPort);

		addrinfo hints;

		::memset(&hints, 0, sizeof(addrinfo));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;
		hints.ai_flags = AI_PASSIVE;

		int iResult = 0;
		addrinfo *result = NULL;

		iResult = ::getaddrinfo(NULL, port, &hints, &result);
		if (iResult != 0)
		{
			//std::cerr << "getaddrinfo failed : " << iResult << std::endl;
			release();

			return 1;
		}

		// result ...
		//std::clog << std::endl;
		ai = result;

		setSocket(::socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol));
		if (getSocket() == INVALID_SOCKET)
		{
			//std::cerr << "Error at socket() : " << ::WSAGetLastError() << std::endl;
			::freeaddrinfo(ai);

			release();

			return 2;
		}

		//std::clog << "createsocket success" << std::endl;
		// successful result..
		return 0;
	}

	int bind(void)
	{
		int iResult = 0;
		iResult = ::bind(getSocket(), ai->ai_addr, (int)ai->ai_addrlen);
		if (iResult == SOCKET_ERROR)
		{
			//std::cerr << "bind faild with error : " << ::WSAGetLastError() << std::endl;
			::freeaddrinfo(ai);

			::closesocket(getSocket());

			release();

			return 1;
		}

		//std::clog << "bind success" << std::endl;
		return 0;
	}

	int listen(void)
	{
		if (::listen(getSocket(), SOMAXCONN) == SOCKET_ERROR)
		{
			//std::cerr << "listen failed with error : " << ::WSAGetLastError() << std::endl;

			::closesocket(getSocket());

			release();

			return 1;
		}

		//std::clog << "listen success" << std::endl;
		return 0;
	}

	int accept(void)
	{
		clientinfo ci;
		//SOCKET clientSocket = INVALID_SOCKET;

		//ci.setSocket(::accept(getSocket(), ci.getAIRef().ai_addr, (int*)&ci.getAIRef().ai_addrlen));
		ci.setSocket(::accept(getSocket(), NULL, NULL));
		if (ci.getSocket() == INVALID_SOCKET)
		{
			//std::cerr << "accept failed : " << ::WSAGetLastError() << std::endl;

			::closesocket(getSocket());

			release();

			return 1;
		}

		clientList.push_back(ci);

		//std::clog << "accept success" << std::endl;
		return 0;
	}

	int close(void)
	{
		int iResult = 0;

		list<clientinfo>::iterator it;
		for (it = clientList.begin(); it != clientList.end(); it++)
		{
			iResult = ::shutdown(it->getSocket(), SD_SEND);
			if (iResult == SOCKET_ERROR)
			{
				//std::cerr << "clientSocket shutdown failed : " << ::WSAGetLastError() << std::endl;

				::closesocket(it->getSocket());
			}
		}

		iResult = ::shutdown(getSocket(), SD_SEND);
		if (iResult == SOCKET_ERROR)
		{
			//std::cerr << "shutdown failed : " << ::WSAGetLastError() << std::endl;

			::closesocket(getSocket());

			release();

			return 1;
		}

		//std::clog << "close success" << std::endl;
		return 0;
	}
public :
	const list<clientinfo>& getClientList(void) const
	{
		return clientList;
	}

private :
	void setport(const char* _szPort)
	{
		if (_szPort)
		{
			::strcpy_s(port, 10, _szPort);
		}
	}
private :

	char port[10];
	char clientaddr[20]; // IPv4

	list<clientinfo> clientList;

	addrinfo* ai;
};