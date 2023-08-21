#include "servertcp.h"

int main()
{
	serverTCP server;

	if (server.BindSock(server.port))
	{
		if (server.ListenSock())
		{
			while (true)
			{
				server.acceptConn();
			}
			return 1;
		}
		return 1;
	}

	server.~serverTCP();
	return 0;
}