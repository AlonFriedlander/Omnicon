//Server Code

#include <PNet\IncludeMe.h>
#include <iostream>

using namespace PNet;

bool ProcessPacket(Packet& packet)
{
	switch (packet.GetPacketType()) {
	case PacketType::PT_ChatMessage:
	{
		std::string chatmessage;
		packet >> chatmessage;
		std::cout << "chat message: " << chatmessage << std::endl;
		break;
	}
	case PacketType::PT_IntegerArray:
	{
		uint32_t arraySize = 0;
		packet >> arraySize;
		for (uint32_t i = 0; i < arraySize; ++i) {
			uint32_t element = 0;
			packet >> element;
			std::cout << "Element[" << i << "] - " << element << std::endl;
		}
		
		break;
	}
	default:
		return false;
	}
	return true;
}

int main() {
	if (Network::Initialize()) {
		std::cout << "winsock api success intialized"<<std::endl;

		//server to listen for connections on port 4790
		//socket - bind to 4790 (listen to this port)

		Socket socket;
		if (socket.Create() == PResult::P_Success) {
			std::cout << "socket succesfuly created" << std::endl;
			if (socket.Listen(IPEndPoint("0.0.0.0", 4790)) == PResult::P_Success) {
				std::cout << "socket succesfuly listen to port 4790" << std::endl;
				
				Socket newConnection;
				
				if (socket.Accept(newConnection) == PResult::P_Success) {
					std::cout << "new connection accepted" << std::endl;

					Packet packet;

					while (true) {
						PResult result = newConnection.Recv(packet);
						if (result != PResult::P_Success) {
							break;
						}
						if (!ProcessPacket(packet)) {
							break;
						}

					}
					newConnection.Close();
				}
				else {
					std::cout << "failed to accept new connection" << std::endl;
				}

			}
			else {
				std::cout << "socket failed to listening to port 4790" << std::endl;
			}
			socket.Close();
		}
		else {
			std::cout << "socket failed to create" << std::endl;
		}
	}
	Network::ShutDown();
	system("pause");
	return 0;
}