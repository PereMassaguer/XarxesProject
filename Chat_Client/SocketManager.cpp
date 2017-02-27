#include "SocketManager.h"



SocketManager::SocketManager()
{
	ip = sf::IpAddress::getLocalAddress();
	EraseBuffer();
}


SocketManager::~SocketManager()
{
}


void SocketManager::ClientInit()
{
	sf::Socket::Status status;
	do {
		status = socket.connect(ip, 5000, sf::seconds(0.5f));
	} while (status != sf::Socket::Done);
}

void SocketManager::SocketReceive() {
	std::size_t bytesReceived;
	socket.setBlocking(false);

	while (1) {
		sf::Socket::Status status = socket.receive(&buffer, sizeof(buffer), bytesReceived);

		if (status == sf::Socket::NotReady) continue;
		else if (status == sf::Socket::Done) {
			std::string string = buffer;
			string.substr(0, bytesReceived);
		}
		else if (status == sf::Socket::Disconnected) break;
	}
}

void SocketManager::SendMessage(std::string message)
{
	size_t sentBytes;
	socket.send(message.c_str(), message.length() + 1, sentBytes);
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
	socket.disconnect();
}