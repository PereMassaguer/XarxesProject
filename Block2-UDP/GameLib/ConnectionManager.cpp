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
		_playerId = 0;
	}

}

void ConnectionManager::Send(std::string str) {
	if (_connectionType == 's') {
		for (auto it : _clients) {
			ConnectionData auxConnection = it.connectionData;
			if (_socket.send(str.c_str(), str.size(), auxConnection.first, auxConnection.second) != sf::Socket::Done)
				Debug("Failure sending: " + str);
			else { Debug("Sent: " + str); }
		}
	}
	else if (_connectionType == 'c') {
		if (_socket.send(str.c_str(), str.size(), _server.first, _server.second) != sf::Socket::Done)
			Debug("Failure sending: " + str);
		else { Debug("Sent: " + str); }
	}
}


void ConnectionManager::Send(std::string str, int playerID) {
	for (auto it : _clients) {
		if (it.id == playerID) {
			ConnectionData auxConnection = it.connectionData;
			if (_socket.send(str.c_str(), str.size(), auxConnection.first, auxConnection.second) != sf::Socket::Done)
				Debug("Failure sending: " + str);
			else { Debug("Sent: " + str); }
			break;
		}
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
			std::string message = _buffer;
			message.c_str();
			Debug("Received: " + message);
			
			if (message != "") {
				if (message == "HELLO") {
					Player tempPlayer;
					tempPlayer.connectionData = _aux;
					tempPlayer.id = _playerId;
					_playerId++;
					_clients.push_back(tempPlayer);
					Debug("New client added; Ip: " + _aux.first.toString() + " Port: " + std::to_string(_aux.second));
					CM.Send("WELCOME_" + std::to_string(tempPlayer.id), tempPlayer.id);
				}
				else if (message.substr(0, message.find("_")) == "HELLO") {
					
					CM.Send("WELCOME_0");
				}
			}
			CM.EraseBuffer();
		}
	}
}

std::string ConnectionManager::GetBuffer()
{
	return std::string(_buffer);
}


void ConnectionManager::EraseBuffer() {
	for (int i = 0; i < sizeof(_buffer); i++)
		_buffer[i] = '\0';
}