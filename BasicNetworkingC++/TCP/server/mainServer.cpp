#include <iostream>
#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

int main()
{
	//intialize winsock
	WSADATA wsData;
	int result = WSAStartup(MAKEWORD(2, 2), &wsData);
	if (result != 0) {
		cerr << "failed to start up the winsock API" << endl;
		return 0;
	}

	//create socket
	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
	if (listening == INVALID_SOCKET){
		cerr << "failed to create socket" << endl;
		return 0;
	}

	//bind the ip address and  port to an socket
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(54000);
	hint.sin_addr.S_un.S_addr = INADDR_ANY; //could also use the inet_pton
	
	bind(listening, (sockaddr*)&hint, sizeof(hint));

	//tell wicnsock to listen
	listen(listening, SOMAXCONN);


	//wait for connection
	sockaddr_in client;
	int clientSize = sizeof(client);

	SOCKET clientSocket = accept(listening, (sockaddr*)&client, &clientSize);

	char host[NI_MAXHOST]; //clients remote name
	char service[NI_MAXHOST]; //service( i.e port) the client is connect on

	ZeroMemory(host, NI_MAXHOST);
	ZeroMemory(service, NI_MAXHOST); //like memset(host, 0 , NI_MAXHOST)

	if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
	{
		cout << host << " connected on port " << service << endl;
	}
	else
	{
		inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
		cout << host << " connected on port " <<
			ntohs(client.sin_port) << endl;
	}

	//close listening socket
	closesocket(listening);

	//while loop : accept and echo message back to client
	char buf[4096];

	while (true)
	{
		ZeroMemory(buf, 4096);

		// Wait for client to send data
		int bytesReceived = recv(clientSocket, buf, 4096, 0);
		if (bytesReceived == SOCKET_ERROR)
		{
			cerr << "Error in recv(). Quitting" << endl;
			break;
		}

		if (bytesReceived == 0)
		{
			cout << "Client disconnected " << endl;
			break;
		}

		cout << string(buf, 0, bytesReceived) << endl;

		// Echo message back to client
		send(clientSocket, buf, bytesReceived + 1, 0);
	}

	//close socket
	closesocket(clientSocket);

	//clieanup winsock
	WSACleanup();

	system("pause");

	return 0;
}


/*
#ifdef _WIN32#include <winsock2.h>#include <ws2tcpip.h>#pragma comment(lib, "ws2_32.lib")
// Link with ws2_32.lib#else#include
<sys/socket.h>#include <netinet/in.h>#include <arpa/inet.h>#include <unistd.h> // For close()#endif


*/