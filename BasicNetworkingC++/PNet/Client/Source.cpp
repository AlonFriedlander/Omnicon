//Client Code

#include <PNet\IncludeMe.h>
#include <iostream>

using namespace PNet;

int main() {
	if (Network::Initialize()) {
		std::cout << "winsock api success intialized" << std::endl;
		Socket socket;
		if (socket.Create() == PResult::P_Success) {
			std::cout << "socket succesfuly created" << std::endl;

			if (socket.Connect(IPEndPoint("127.0.0.1", 4790)) == PResult::P_Success) {
				std::cout << "succesfuly connect to the server!" << std::endl;
				
				Packet stringPacket(PacketType::PT_ChatMessage);
				stringPacket << std::string("this is my string packet!");

				Packet integersPacket(PacketType::PT_IntegerArray);
				uint32_t arraySize = 6;
				uint32_t integerArray[6] = { 2,5,7,5,2,9 };
				integersPacket << arraySize;
				for (auto integer : integerArray) {
					integersPacket << integer;
				}

				
				while (true) {
					PResult result;
					if ((rand() % 2) == 0) {
						result = socket.Send(stringPacket);
					}
					else {
						result = socket.Send(integersPacket);
					}

					if (result != PResult::P_Success) {
						break;
					}
					std::cout << "attempting so send chunk of data!" << std::endl;
					Sleep(500);
				}
			}
			else {
				std::cout << "connect failed to create" << std::endl;
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