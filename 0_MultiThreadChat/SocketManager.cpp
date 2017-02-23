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
	sf::Socket::Status status = listener.listen(CONNECT_PORT);
	if (status != sf::Socket::Done) {
		std::cout << "Connection failed on port " << (int)CONNECT_PORT << std::endl;
		return;
	}
	listener.setBlocking(false);

	std::time_t initTime = time(NULL);
	while (initTime + 5 > time(NULL)) {
		sf::Socket::Status check;
		check = listener.accept(socket);
	}
}

void SocketManager::ClientInit()
{
	sf::Socket::Status status;
	do {
		status = socket.connect(ip, 5000, sf::seconds(0.5f));
	} while (status != sf::Socket::Done);
	socket.setBlocking(false);
}

void SocketManager::SocketReceive() {
	std::size_t bytesReceived;

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
	socket.send(message.c_str(), message.length() + 1);
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
	socket.disconnect();
}