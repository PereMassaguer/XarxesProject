#include "ConnectionManager.h"


/*funcs:
	tryPing
	addplayer
	checkConn
	DisconnectClient
	DisconnectAll
	
*/
ConnectionManager::ConnectionManager()
{
}


ConnectionManager::~ConnectionManager()
{
}


void ConnectionManager::Init(char connectionType) {
	_connectionType = connectionType;	

	_server.first = sf::IpAddress::getLocalAddress();
	_server.second = PORT;
	if (_connectionType == 'c') {
		std::string t = "HELLO";
		_socket.send(t.c_str(), t.size(), sf::IpAddress::getLocalAddress(), PORT);
	}
	else if (_connectionType == 's') {
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


void ConnectionManager::Send(std::string str, ConnectionData recvData) {

		if (_socket.send(str.c_str(), str.size(), recvData.first, recvData.second) != sf::Socket::Done)
				Debug("Failure sending: " + str);
		else { Debug("Sent: " + str); }
}


void ConnectionManager::Recv() {
	std::size_t received;
	_socket.setBlocking(true);
	ConnectionData recvData;

	while (true) {
		if (_socket.receive(_buffer, BUFFER_SIZE, received, recvData.first, recvData.second) != sf::Socket::Done) {
			Debug("Failure receiving");
		}
		else {
			std::string message = _buffer;
			message.c_str();
			Debug("Received: " + message);
			
			if (message != "") {
				if (message == "HELLO") {
					//void AddPlayer();
					Player tempPlayer;
					tempPlayer.connectionData = recvData;
					tempPlayer.id = _playerId;
					tempPlayer.lastConCheck = GetTickCount();
					_playerId++;
					//-----
					_clients.push_back(tempPlayer);
					Debug("New client added; Ip: " + recvData.first.toString() + " Port: " + std::to_string(recvData.second));
					CM.Send("WELCOME_" + std::to_string(tempPlayer.id), recvData);
				}
				else if (message == "ACK") {
					for (auto it : _clients) {
						if (it.connectionData == recvData) {
							it.lastConCheck = GetTickCount();
							break;
						}
					}
				}
				else if (message.substr(0, message.find("_")) == "HELLO") {
					std::string name = message.substr(message.find("_") + 1, message.size()); 
					CM.Send("WELCOME_" + name, recvData);
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