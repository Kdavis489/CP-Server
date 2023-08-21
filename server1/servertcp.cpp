#include "servertcp.h"

serverTCP::serverTCP()
{
	initSock();
}

serverTCP::~serverTCP()
{

	closesocket(serverSocket);
}

bool serverTCP::BindSock(int port)
{
	serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (serverSocket == INVALID_SOCKET)
	{
		std::cerr << "Socket creation failed." << std::endl;
		exit(1);
		return false;
	}

	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = INADDR_ANY;
	serverAddr.sin_port = htons(80);

	if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
	{
		std::cerr << "Bind failed." << std::endl;
		closesocket(serverSocket);
		cleanupSock();
		exit(1);
		return false;
	}
	FD_ZERO(&readSet);
	FD_SET(serverSocket, &readSet);
	std::cout << "Socket successfully bond" << std::endl;
	return true;
}

bool serverTCP::ListenSock()
{
	if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR)
	{
		closesocket(serverSocket);
		cleanupSock();
		exit(1);
		return false;
	}
	std::cout << " Server is listening for incoming connections.." << std::endl;
	return true;
}

bool serverTCP::acceptConn()
{
	while (true)
	{
		fd_set tempReadSet = readSet;
		int clientSockets = select(serverSocket + 1, &tempReadSet, nullptr, nullptr, nullptr);
		if (clientSockets == SOCKET_ERROR)
		{
			std::cerr << "Select error." << std::endl;
			break;
		}

		if (FD_ISSET(serverSocket, &tempReadSet))
		{
			clientSocket = accept(serverSocket, nullptr, nullptr);
			if (clientSocket == INVALID_SOCKET)
			{
				std::cerr << "Accept failed." << std::endl;
			}
			else
			{
				std::cout << "New connection accepted." << std::endl;
				FD_SET(clientSocket, &readSet);
			}
		}
	}
	Run();
	return true;
}

void serverTCP::Run()
{
	fd_set tempReadSet = readSet;
	//handle data
	for (int i = 0; i <= serverSocket; ++i)
	{
		if (FD_ISSET(i, &tempReadSet))
		{
			if (i == serverSocket)
			{
				continue;
			}
			bytesRead = recv(i, buffer, sizeof(buffer), 0);
			if (bytesRead <= 0)
			{
				std::cout << "client disconnected." << std::endl;
			}
			else
			{
				std::cerr << "Receive error." << std::endl;
			}
			closesocket(i);
			FD_CLR(i, &readSet);
		}
		else
		{
			buffer[bytesRead] = '\0';
			std::cout << "Received: " << buffer << std::endl;
		}
	}
}

void serverTCP::initSock()
{
#ifdef _WIN32
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		std::cerr << "WSAStartup failed." << std::endl;
		exit(1);
	}
#endif
}

void serverTCP::cleanupSock()
{
#ifdef _WIN32
	WSACleanup();
#endif
}
