#pragma once

#include "Socket.h"
#include <list>

struct ClientInfo
{
	typedef ClientInfo Me;

	ClientInfo();
	~ClientInfo();

	unsigned int id() const;
	addrinfo info() const;
	SOCKET& get();
	SOCKET set(const SOCKET& o);

private :
	unsigned int _id;
	SOCKET _Socket;
	addrinfo _ai;

	static unsigned int gen_id;
};

class Server
{
public :
	Server();
	Server(const char* szPort);
	~Server();

	int CreateSocket();
	int CreateSocket(const char* szPort);

	int Bind(void);
	int Listen(void);
	int Accept(void);
	int Close(void);

public :
	std::list<ClientInfo*>& GetClientList() const;

private :
	void _setport(const char* szPort);
	void _createsocket();
	void _release();
private :
	Socket m_socket;

	char* port;
	char clientaddr[20]; // IPv4

	std::list<ClientInfo*> ClientList;

	addrinfo* ai;
};