#include "Socket.h"

Socket::Socket()
: _Socket(INVALID_SOCKET)
{}
Socket::~Socket()
{}
int Socket::Initialize()
{
	int ret;

	ret = ::WSAStartup(MAKEWORD(2,2), &_wsaData);
	if (ret != 0)
		return ret;

	if (LOBYTE(_wsaData.wVersion) != 2 ||
		HIBYTE(_wsaData.wVersion) != 2)
	{
		::WSACleanup();

		return 1;
	}

	return 0;
}
void Socket::Release()
{
	::closesocket(_Socket);
	::WSACleanup();
}
SOCKET& Socket::get()
{
	return _Socket;
}
SOCKET Socket::set(const SOCKET& o)
{
	SOCKET prev = _Socket;
	_Socket = o;

	return prev;
}