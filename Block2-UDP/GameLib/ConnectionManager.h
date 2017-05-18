#pragma once


#include "SFML\Network\UdpSocket.hpp"
#include "ID.h"
#include <map>


#include <time.h>

#include <Windows.h>

#define CM ConnectionManager::Instance()
class ConnectionManager
{
	char _buffer[BUFFER_SIZE];
	sf::UdpSocket _socket;
	ConnectionData _server;
	std::map<ConnectionData, Player> _clients;
	std::vector<Player> _players;
	char _connectionType;


	int _playerId;
public:
	ConnectionManager();
	inline static ConnectionManager &Instance() {
		static ConnectionManager socketManager;
		return socketManager;
	}
	~ConnectionManager();
	void InitServer();
	void InitClient(std::string name);
	void Send(std::string str);
	void Send(std::string str, ConnectionData recvData);
	void Recv();

	std::string GetBuffer();
	void EraseBuffer();
	Coordinate RandomFreeCoord();
	Player CreatePlayer(std::string name);
	std::vector<std::vector<Coordinate>> GetPlayersCoords();
	void Move(int d);
};