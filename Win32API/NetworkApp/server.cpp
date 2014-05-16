#include "Server.h"

////////////////////////////////////////////////////////////
// ClientInfo
////////////////////////////////////////////////////////////
unsigned int ClientInfo::gen_id = 0;

ClientInfo::ClientInfo()
: id(gen_id++), Socket(INVALID_SOCKET)
{}
ClientInfo::~ClientInfo()
{}

unsigned int ClientInfo::id() const
{
	return _id;
}
addrinfo ClientInfo::info() const
{
	return _ai;
}
SOCKET& ClientInfo::Socket()
{
	return _Socket;
}
SOCKET ClientInfo::Socket(const SOCKET& o)
{
	SOCKET prev = _Socket;
	_Socket = o;
	return prev;
}

////////////////////////////////////////////////////////////
// Server
////////////////////////////////////////////////////////////
Server::Server()
: port(NULL), ai(NULL)
{}
Server::Server(const char* szPort)
: port(NULL), ai(NULL)
{
	_setport(szPort);
}
Server::~Server()
{
	SafeDelete(port);
}

int Server::CreateSocket()
{
	return _createsocket();
}
int Server::CreateSocket(const char* szPort)
{
	setport(szPort);

	return _createsocket();
}

void Server::_setport(const char* szPort)
{
	strAlloc(port, szPort);
}
int Server::_createsocket()
{
	if (m_socket.Initialize() != 0)
		return 1;

	addrinfo hints;

	::memset(&hints, 0, sizeof(addrinfo));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	int ret;
	addrinfo* result = NULL;

	ret = ::getaddrinfo(NULL, port, &hints, &result);
	if (ret != 0)
	{
		_release();

		return 1;
	}

	ai = result;

	m_socket.set(::socket(ai->ai_family,
						ai->ai_socktype,
						ai->ai_protocol));
	if (m_socket.get() == INVALID_SOCKET)
	{
		_release();

		return 2;
	}

	return 0;
}
void Server::_release()
{
	if (ai)
		::freeaddrinfo(ai);

	m_socket.Release();
}
int Server::Bind()
{
	int ret = 0;
	ret = ::bind(m_socket.get(), ai->ai_addr, int(ai->ai_addrlen));
	if (ret == SOCKET_ERROR)
	{
		_release();

		return 1;
	}

	return 0;
}
int Server::Listen()
{
	if (::listen(m_socket.get(), SOMAXCONN) == SOCKET_ERROR)
	{
		_release();

		return 1;
	}

	return 0;
}
int Server::Accept()
{
	ClientInfo* ci = new ClientInfo;

	ci->set(::accept(m_socket.get(), NULL, NULL));
	if (ci->get() == INVALID_SOCKET)
	{
		_release();

		delete ci;

		return 1;
	}

	ClientList.push_back(ci);

	return 0;
}
std::list<ClientInfo*>& Server::GetClientList() const
{
	return ClientList;
}
int Server::Close()
{
	int ret = 0;

	std::list<ClientInfo*>::iterator it;
	for (it = ClientList.begin(); it != ClientList.end(); it++)
	{
		ret = ::shutdown((*it)->get(), SD_SEND);
		if (ret == SOCKET_ERROR)
		{
			::closesocket((*it)->get());
		}
	}

	ret = ::shutdown(m_socket.get(), SD_SEND);
	if (ret == SOCKET_ERROR)
	{
		::closesocket(m_socket.get());

		_release();

		return 1;
	}

	return 0;
}