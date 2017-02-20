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
	sf::TcpListener listener;
	listener.listen(5000);
	if (listener.accept(socketS) != sf::Socket::Done)
		std::cout << "accept socket error" << std::endl;

	if (listener.accept(socketR) != sf::Socket::Done)
		std::cout << "accept socket error" << std::endl;
}

void SocketManager::ClientInit()
{
	sf::Socket::Status status;
	do {
		status = socketR.connect(ip, 5000, sf::seconds(0.5f));
	} while (status != sf::Socket::Done);

	do {
		status = socketS.connect(ip, 5000, sf::seconds(0.5f));
	} while (status != sf::Socket::Done);

}

void SocketManager::SocketReceive() {
	std::size_t received;

	while (1) {
		socketR.receive(&buffer, sizeof(buffer), received);
	}
}

void SocketManager::SendMessage(std::string message)
{
	socketS.send(message.c_str(), message.length() + 1);
}

char* SocketManager::getBuffer()
{
	return buffer;
}

void SocketManager::EraseBuffer() {
	char t[2] = "";
	strcpy_s(buffer, t);
}

void SocketManager::Init(char m)
{
	mode = m;
	if (mode == 's')
		ServerInit();
	else if (mode == 'r')
		ClientInit();
}

void SocketManager::Disconnect() {
	socketS.disconnect();
	socketR.disconnect();
}