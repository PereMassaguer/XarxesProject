#pragma once


#include "SFML\Network\UdpSocket.hpp"
#include "ID.h"
#include <map>

#define CM ConnectionManager::Instance()
class ConnectionManager
{
	struct Player {
		int id;
		std::string name;
		Coordinate coord; 
		ConnectionData connectionData;
	};

	char _buffer[BUFFER_SIZE];
	sf::UdpSocket _socket;
	ConnectionData _server;
	std::vector<Player> _clients;
	char _connectionType;


	int _playerId;
public:
	ConnectionManager();
	inline static ConnectionManager &Instance() {
		static ConnectionManager socketManager;
		return socketManager;
	}
	~ConnectionManager();
	void Init(char connectionType);
	void Send(std::string str);
	void Send(std::string str, int playerID);
	void Recv();

	std::string GetBuffer();
	void EraseBuffer();
};