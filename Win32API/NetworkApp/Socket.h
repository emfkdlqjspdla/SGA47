#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif // WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>

#pragma comment(lib, "ws2_32.lib")

class Socket
{
public :
	Socket();
	virtual ~Socket();

	int Initialize(void);
	void Release(void);

	SOCKET& get();
	SOCKET set(const SOCKET& o);

private :
	WSADATA _wsaData;
	SOCKET _Socket;

};