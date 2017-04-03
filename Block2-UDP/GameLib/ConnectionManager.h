#pragma once


#include "SFML\Network\UdpSocket.hpp"
#include "ID.h"
#include <map>

#define CM ConnectionManager::Instance()
class ConnectionManager
{
	char _buffer[BUFFER_SIZE];
	sf::UdpSocket _socket;
	std::pair<sf::IpAddress, unsigned short> _server;
	std::pair<sf::IpAddress, unsigned short> _clients;
	char _connectionType;
public:
	ConnectionManager();
	inline static ConnectionManager &Instance() {
		static ConnectionManager socketManager;
		return socketManager;
	}
	~ConnectionManager();
	void Init(char connectionType);
	void Send(std::string str);
	void Recv();

	std::string GetBuffer();
	void EraseBuffer();
};