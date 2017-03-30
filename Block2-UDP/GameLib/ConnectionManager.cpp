#include "ConnectionManager.h"



ConnectionManager::ConnectionManager()
{
}


ConnectionManager::~ConnectionManager()
{
}


void ConnectionManager::Init(char connectionType) {
	_connectionType = connectionType;
	
	unsigned short port = _connectionType == 's' ? PORT : PORT + 1;

	_sockSend.first = sf::IpAddress::getLocalAddress();
	_sockSend.second = _connectionType != 's' ? PORT : PORT + 1;

	sf::Socket::Status st = _socket.bind(port);
	if (st != sf::Socket::Done)
		Debug("Failed socket binding");
}

void ConnectionManager::Send(std::string str) {
	if (_socket.send(str.c_str(), str.size(), _sockSend.first, _sockSend.second) != sf::Socket::Done)
		Debug("Failure sending: " + str);
	else { Debug("Sent: " + str); }
}


void ConnectionManager::Recv() {
	std::size_t received;
	_socket.setBlocking(true);
	while (true) {
		if (_socket.receive(_buffer, BUFFER_SIZE, received, _server.first, _server.second) != sf::Socket::Done) {
			Debug("Failure receiving");
		}
		else {
			std::string t = _buffer;
			t.c_str();
			Debug("Received: " + t);
		}
	}
}

std::string ConnectionManager::GetBuffer()
{
	return std::string(_buffer);
}


void ConnectionManager::EraseBuffer() {
	char t[2] = "";
	strcpy_s(_buffer, t);
}