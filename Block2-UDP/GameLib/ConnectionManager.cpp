#include "ConnectionManager.h"



ConnectionManager::ConnectionManager()
{
}


ConnectionManager::~ConnectionManager()
{
}


void ConnectionManager::Init(char connectionType) {
	_connectionType = connectionType;	

	if (_connectionType == 'c') {
		std::string t = "HELLO";
		_socket.send(t.c_str(), t.size(), sf::IpAddress::getLocalAddress(), PORT);
	}
	else if (_connectionType == 's') {
		_server.first = sf::IpAddress::getLocalAddress();
		_server.second = PORT;
		sf::Socket::Status st = _socket.bind(_server.second);
		if (st != sf::Socket::Done)
			Debug("Failed socket binding");
	}

}

void ConnectionManager::Send(std::string str) {
	if (_connectionType == 's') {
		if (_socket.send(str.c_str(), str.size(), _clients.first, _clients.second) != sf::Socket::Done)
			Debug("Failure sending: " + str);
		else { Debug("Sent: " + str); }
	}
	else if (_connectionType == 'c') {
		if (_socket.send(str.c_str(), str.size(), _server.first, _server.second) != sf::Socket::Done)
			Debug("Failure sending: " + str);
		else { Debug("Sent: " + str); }
	}
}


void ConnectionManager::Recv() {
	std::size_t received;
	_socket.setBlocking(true);
	std::pair<sf::IpAddress, unsigned short> _aux;

	while (true) {
		if (_socket.receive(_buffer, BUFFER_SIZE, received, _aux.first, _aux.second) != sf::Socket::Done) {
			Debug("Failure receiving");
		}
		else {
			std::string t = _buffer;
			t.c_str();
			Debug("Received: " + t);
			
			if (t == "HELLO") {
				_clients = _aux;
				Debug("New client added");
			}
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