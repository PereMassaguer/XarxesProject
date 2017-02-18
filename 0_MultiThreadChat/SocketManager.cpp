#include "SocketManager.h"



SocketManager::SocketManager()
{
	ip = sf::IpAddress::getLocalAddress();
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

	//listenerB.listen(5001);//borrar
	if (listener.accept(socketR) != sf::Socket::Done)
		std::cout << "accept socket error" << std::endl;

	sf::Thread getIncomingMessage(&SocketManager::SocketReceive, this);
	getIncomingMessage.launch();
}

void SocketManager::ClientInit()
{
	sf::Socket::Status status;
	do {
		status = socketR.connect(ip, 5000, sf::seconds(0.5f));
	} while (status != sf::Socket::Done);

	do {
		status = socketS.connect(ip, 5001, sf::seconds(0.5f));
	} while (status != sf::Socket::Done);

	sf::Thread getClientMessage(&SocketManager::SocketReceive, this);
	getClientMessage.launch();
}

void SocketManager::SocketReceive() {
	std::size_t received;

	while (1) {
		if (mode == 's')
			socketR.receive(&buffer, sizeof(buffer), received);
		else if (mode == 'r')
			socketS.receive(&buffer, sizeof(buffer), received);
		std::cout << buffer << std::endl;
	}
}

void SocketManager::SendMessage(std::string message)
{
	if (mode == 's')
		socketS.send(message.c_str(), message.length() + 1);
	else if(mode == 'r')
		socketR.send(message.c_str(), message.length() + 1);
}

char* SocketManager::getBuffer()
{
	return buffer;
}

void SocketManager::Init(char m)
{
	mode = m;
	if (mode == 's')
		ServerInit();
	else if (mode == 'r')
		ClientInit();
}