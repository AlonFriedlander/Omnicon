#pragma once

#include <string>
#include <WS2tcpip.h> //header file for all winsock functions

#pragma comment(lib, "ws2_32.lib") //winsock library files
#define MAX_BUFFER_SIZE (49152)
//forward delcare
class CTcpListener;

//callback to data received
typedef void (*MessageRecievedHandler)(CTcpListener *listener, int socketId, std::string msg);

class CTcpListener
{

public:

	// Constructor
	CTcpListener(std::string ipAddress, int port, MessageRecievedHandler handler);

	// Destructor
	~CTcpListener();

	// Send a message to the specified client
	void Send(int clientSocket, std::string msg);

	// Initialize winsock
	bool Init();

	// The main processing loop
	void Run();

	// Clean up after using the service
	void Cleanup();

private:

	// Create a socket
	SOCKET CreateSocket();

	// Wait for a connection
	SOCKET WaitForConnection(SOCKET listening);

	// Address of the server
	std::string				m_ipAddress;

	// Listening port
	int						m_port;

	// Message received event handler
	MessageRecievedHandler	MessageReceived;
};