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


	sf::TcpSocket socket;
	std::time_t initTime = time(NULL);
	while (initTime + 5 > time(NULL)) {
		sf::Socket::Status check;
		check = listener.accept(socket);
	}
	clientSockets.push_back(&socket);
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
	if (sizeof(message.c_str()) < sentBytes) {

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
	socket.disconnect();
}