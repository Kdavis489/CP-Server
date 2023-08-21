#pragma once
#include <iostream>
#include <cstring>

#ifdef _WIN32
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")
#define socklen_t int
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#define SOCKET int
#define INVALID_SOCKET -1
#define closesocket close
#define SOCKET_ERROR -1
#endif

class serverTCP
{
private:
	SOCKET serverSocket;
	SOCKET clientSocket;
	SOCKET clientSockets;
	fd_set readSet;
	char buffer[1024];
	int bytesRead;
public:
	serverTCP();
	~serverTCP();
	bool BindSock(int port);
	bool ListenSock();
	bool acceptConn();
	void Run();
	// std::string receivePacket
	//void sendpacket();
	const int port = 80;
private:
	void initSock();
	void cleanupSock();
};