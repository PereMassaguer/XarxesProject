#include "ConnectionManager.h"



ConnectionManager::ConnectionManager()
{
}


ConnectionManager::~ConnectionManager()
{
}


void ConnectionManager::Init(char connectionType) {
	connectionType = _connectionType;

	
	sf::Socket::Status st = _socket.bind(_connectionType == 's' ? PORT : PORT + 1);
	if (st != sf::Socket::Done)
		Debug("Failed socket binding");
}

void ConnectionManager::Send(std::string str) {
	if (_socket.send(str.c_str(), str.size(), _server.first, _server.second) != sf::Socket::Done)
		Debug("Failure sending: " + str);
	else { Debug("Sent: " + str); }
}


void ConnectionManager::Recv() {
	std::size_t received;
	if(_socket.receive(_buffer, BUFFER_SIZE, received, _server.first, _server.second) != sf::Socket::Done)
		Debug("Failure receiving");
	else { Debug("Received: " + *_buffer); }
}

std::string ConnectionManager::GetBuffer()
{
	return std::string(_buffer);
}


void ConnectionManager::EraseBuffer() {
	char t[2] = "";
	strcpy_s(_buffer, t);
}