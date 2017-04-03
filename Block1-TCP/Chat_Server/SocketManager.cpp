#include "SocketManager.h"



SocketManager::SocketManager()
{
	ip = sf::IpAddress::getLocalAddress();
	EraseBuffer(0);
	EraseBuffer(1);
}


SocketManager::~SocketManager()
{
	for (auto it : clientSockets) it->disconnect();
	clientSockets.clear();
}


void SocketManager::ServerInit()
{
	std::cout << "Server ON" << std::endl;
	sf::Socket::Status status = listener.listen(CONNECT_PORT);
	if (status != sf::Socket::Done) {
		std::cout << "Connection failed on port " << (int)CONNECT_PORT << std::endl;
		return;
	}
	selector.add(listener);
	SayConnections();
}


void SocketManager::SocketReceive() {
	std::size_t bytesReceived;

	while (1) {
		if (selector.wait()) {
			if (selector.isReady(listener)) {
				sf::TcpSocket* client = new sf::TcpSocket;
				if (listener.accept(*client) == sf::Socket::Done) {
					clientSockets.push_back(client);
					selector.add(*client);
					std::cout << "New client connected: "; SayConnections();
				}
				else { 
					delete client; 
				}
			}
			else {
				std::vector<int> eraseList;
				for (int i = 0; i < clientSockets.size();i++) {
					if (selector.isReady(*clientSockets[i])) {
						sf::TcpSocket::Status st;
						st = clientSockets[i]->receive(&buffer[i], sizeof(buffer[i]), bytesReceived);
						if (st == sf::TcpSocket::Disconnected) {
							std::cout << "Client disconnected, ";
							eraseList.push_back(i);
						}
						SendMessage(buffer[i]);
					}
				}
				for (auto it : eraseList) {
					clientSockets.erase(clientSockets.begin() + it);
					SayConnections();
				}
			}
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
	}
}

void SocketManager::SendMessage(std::string message)
{
	size_t sentBytes;
	std::cout << "Sent: " << message << std::endl;
	for (auto it : clientSockets) {
		it->send(message.c_str(), message.length() + 1, sentBytes);
	}
}

char* SocketManager::getBuffer(int i)
{
	return buffer[i];
}

void SocketManager::EraseBuffer(int i) {
	char t[2] = "\0";
	strcpy_s(buffer[i], t);
}


void SocketManager::SayConnections()
{
	std::cout << clientSockets.size() << " clients connected now" << std::endl;
}
