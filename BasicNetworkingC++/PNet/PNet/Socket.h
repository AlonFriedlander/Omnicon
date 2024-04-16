#pragma once
#include "SocketHandle.h"
#include "PResult.h"
#include "IPVersion.h"
#include "SocketOption.h"
#include "IPEndPoint.h"
#include "Constants.h"
#include "Packet.h"

namespace PNet
{
	class Socket {
	public:
		Socket(IPVersion ipversion = IPVersion::IPv4,
			SocketHandle handle = INVALID_SOCKET);
		PResult Create();
		PResult Close();
		PResult Bind(IPEndPoint endpoint);
		PResult Listen(IPEndPoint endpoint, int backlog = 5);
		PResult Accept(Socket &outSocket);
		PResult Connect(IPEndPoint endpoint);

		PResult Send(const void *data, int numerOfBytes, int &bytesSent);
		PResult Send(Packet &packet);
		PResult SendAll(const void *data, int numerOfBytes);

		PResult Recv(void *desitnation, int numerOfBytes, int &bytesReceived);
		PResult Recv(Packet &packet);
		PResult RecvAll(void *desitnation, int numerOfBytes);
		SocketHandle GetHandle();
		IPVersion GetIpVersion();

	private:
		PResult SetSocketOption(SocketOption option, BOOL value);
		IPVersion ipversion = IPVersion::IPv4;
		SocketHandle handle = INVALID_SOCKET;
	};
}