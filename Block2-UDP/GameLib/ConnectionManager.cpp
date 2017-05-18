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
	_playerId = 0;
}


ConnectionManager::~ConnectionManager()
{
}


void ConnectionManager::InitServer() {
	_connectionType = 's';
	_server = ConnectionData(sf::IpAddress::getLocalAddress(), PORT);

	sf::Socket::Status st = _socket.bind(_server.second);
	if (st != sf::Socket::Done)
		Debug("Failed socket binding");


}

void ConnectionManager::InitClient(std::string name)
{
	_connectionType = 'c';

	_server = ConnectionData(sf::IpAddress::getLocalAddress(), PORT);

	_players.push_back(Player());
	_players[0].name = name;
	_players[0].stackedTail = 2;

	std::string t = "HELLO_" + _players[0].name;
	Send(t);
}

void ConnectionManager::Send(std::string str) {
	if (_connectionType == 's') {
		for (auto it : _clients) {
			ConnectionData auxConnection = it.first;
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
				//Server
				if (_connectionType == 's') {
					if (message.substr(0, message.find("_")) == "HELLO") {
						Player newPlayer = CreatePlayer(message.substr(message.find("_"), message.size()));
						_clients.insert(std::pair<ConnectionData, Player>(recvData, newPlayer));

						Debug("New client added; Ip: " + recvData.first.toString() + " Port: " + std::to_string(recvData.second));
						
						CM.Send("WELCOME_" + StringedIDCoord(newPlayer.id, newPlayer.tail[0]), recvData);
					}
					else if (message == "ACK") {
						_clients[recvData].lastConCheck = GetTickCount();
					}
					else if (message == "GOTPOS") {
						Player newPlayer = _clients[recvData];
						for (auto it : _clients) {
							CM.Send("PLAYERI_" + StringedIDCoord(it.second.id, it.second.tail), recvData);
							CM.Send("PLAYERI_" + StringedIDCoord(newPlayer.id, newPlayer.tail));
						}
					}
					else if (message.substr(0, message.find("_")) == "MOVE") {
						std::string tStr = message.substr(message.find("_") + 1, message.size());
						int id = stoi(tStr.substr(0, tStr.find("_")));						
						for (auto &it : _clients) {
							if (it.second.id == id) {
								Coordinate tCoord;
								StringToCoord(tCoord, tStr.substr(tStr.find("_") + 1, tStr.size()));
								if (it.second.stackedTail > 0) {
									it.second.tail.push_back(Coordinate()); 
									it.second.stackedTail--;
								}

								for (int i = it.second.tail.size() - 1; i > 0; i--) it.second.tail[i] = it.second.tail[i - 1];
								it.second.tail[0].first += tCoord.first;
								it.second.tail[0].second += tCoord.second;

								CM.Send("MOVE_" + StringedIDCoord(it.second.id, it.second.tail));
								break;
							}
						}
					}
				}
				//Client
				else {
					if (message.substr(0, message.find("_")) == "WELCOME") {
						std::string tempStr = message.substr(message.find("_") + 1, message.size());
						_players[0].tail.push_back(Coordinate());
						StringToIDCoord(_players[0].id, _players[0].tail[0], tempStr);

						CM.Send("GOTPOS", recvData);
					}
					else if (message.substr(0, message.find("_")) == "PLAYERI") {
						Player tPlayer;
						tPlayer.tail.push_back(Coordinate(-10,-10));

						std::string tempStr = message.substr(message.find("_") + 1, message.size());
						StringToIDCoord(tPlayer.id, tPlayer.tail, tempStr);

						bool playerExists = false;
						for (auto it : _players) {
							if (it.id == tPlayer.id) {
								it.tail = tPlayer.tail;
								playerExists = true;
								break;
							}
						}
						if (!playerExists) {
							_players.push_back(tPlayer);
							Debug("Added player(id, coord):" + StringedIDCoord(tPlayer.id, tPlayer.tail[0]));
						}
					}
					else if (message.substr(0, message.find("_")) == "MOVE") {
						std::string tStr = message.substr(message.find("_") + 1, message.size());
						int id = stoi(tStr.substr(0, tStr.find("_")));
						for (auto &it : _players) {
							if (it.id == id) {
								if (it.stackedTail > 0) {
									it.tail.push_back(Coordinate());
									it.stackedTail--;
								}
								for (int i = it.tail.size() - 1; i > 0; i--) it.tail[i] = it.tail[i - 1];
								StringToCoord(it.tail, tStr.substr(tStr.find("_") + 1, tStr.size()));
								break;
							}
						}
					}
				}				
			}
			CM.EraseBuffer();
		}
		for (auto it : _clients) {
			if (it.second.lastMovement > GetTickCount() - 200) {
				if (it.second.stackedTail > 0) {
					it.second.tail.push_back(Coordinate());
					it.second.stackedTail--;
				}

				for (int i = it.second.tail.size() - 1; i > 0; i--) it.second.tail[i] = it.second.tail[i - 1];
				it.second.tail[0].first = 2 * it.second.tail[0].first - it.second.tail[1].first;
				it.second.tail[0].second = 2 * it.second.tail[0].second - it.second.tail[1].second;

				CM.Send("MOVE_" + StringedIDCoord(it.second.id, it.second.tail));
				break;
			}				
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


Coordinate ConnectionManager::RandomFreeCoord() {
	Coordinate rCoord;
	bool freeCoord;
	do {
		freeCoord = true;
		rCoord = Coordinate(rand() % MAP_COLUMNS, rand() % MAP_ROWS);
		for (auto it : _players) {
			for (auto it2 : it.tail) {
				if (it2 == rCoord) {
					freeCoord = false;
					break;
				}
			}
		}
	} while (!freeCoord);

	return rCoord;
}


Player ConnectionManager::CreatePlayer(std::string name) {
	Player tempPlayer;
	tempPlayer.name = name;
	tempPlayer.id = _playerId;
	tempPlayer.lastConCheck = GetTickCount();
	tempPlayer.tail.push_back(Coordinate(RandomFreeCoord()));
	tempPlayer.stackedTail = 2;
	_playerId++;

	return tempPlayer;
}



std::vector<std::vector<Coordinate>> ConnectionManager::GetPlayersCoords() {
	std::vector<std::vector<Coordinate>> tCoord;
	
	for (auto it : _players) {
		std::vector<Coordinate> tTail;
		for (auto it2 : it.tail) tTail.push_back(it2);
		tCoord.push_back(tTail);
	}

	return tCoord;
}


void ConnectionManager::Move(int d) {
	Coordinate tCoord(0, 0);
	if (d == 71)
		tCoord.first--;
	else if(d == 72)
		tCoord.first++;
	else if (d == 73)
		tCoord.second--;
	else if (d == 74)
		tCoord.second++;
	CM.Send("MOVE_" + StringedIDCoord(_players[0].id, tCoord));
}