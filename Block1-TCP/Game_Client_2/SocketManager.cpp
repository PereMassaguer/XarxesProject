#include "SocketManager.h"



SocketManager::SocketManager()
{
	ip = sf::IpAddress::getLocalAddress();
	EraseBuffer();
}


SocketManager::~SocketManager()
{
	socket.disconnect();
}


void SocketManager::ClientInit()
{
	sf::Socket::Status st;
	do {
		st = socket.connect(ip, 5000, sf::seconds(0.5f));
	} while (st != sf::Socket::Done);
}

void SocketManager::SocketReceive() {
	std::size_t bytesReceived;
	
	while (1) {
		status = socket.receive(&buffer, sizeof(buffer), bytesReceived);

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
	std::cout << "Sent: " << message << std::endl;
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

sf::TcpSocket::Status SocketManager::GetConnectionStatus() {
	return status;
}