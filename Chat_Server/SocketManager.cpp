#include "SocketManager.h"



SocketManager::SocketManager()
{
	ip = sf::IpAddress::getLocalAddress();
	EraseBuffer();
}


SocketManager::~SocketManager()
{
}


void SocketManager::ServerInit()
{
	sf::Socket::Status status = listener.listen(CONNECT_PORT);
	if (status != sf::Socket::Done) {
		std::cout << "Connection failed on port " << (int)CONNECT_PORT << std::endl;
		return;
	}
	listener.setBlocking(false);
	selector.add(listener);
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
				}
				else { 
					delete client; 
				}
			}
			else {
				for (auto it : clientSockets) {
					if (selector.isReady(*it)) {
						it->receive(&buffer, sizeof(buffer), bytesReceived);
						SM.SendMessage(buffer);
					}
				}
			}
		}
	}
}

void SocketManager::SendMessage(std::string message)
{
	size_t sentBytes;

	for (auto it : clientSockets) {
		it->send(message.c_str(), message.length() + 1, sentBytes);
	}
}

char* SocketManager::getBuffer()
{
	return buffer;
}

void SocketManager::EraseBuffer() {
	char t[2] = "";
	strcpy_s(buffer, t);
}

void SocketManager::Disconnect() {
	for (auto it : clientSockets) it->disconnect();
}