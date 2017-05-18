#pragma once
#include <SFML\Network.hpp>
#include <iostream>
#include <thread>
#include "ID.h"

#define CONNECT_PORT 5000

#define SM SocketManager::Instance()
class SocketManager
{
	std::vector<Client> _clients;
	
	sf::TcpListener listener;
	sf::SocketSelector selector;

public:
	SocketManager();
	~SocketManager();

	inline static SocketManager &Instance() {
		static SocketManager socketManager;
		return socketManager;
	}

	void ServerInit();
	void SocketReceive();
	void SendMessage(std::string message);

	void SendMessage(std::string message, int i);

	char* getBuffer(int i);
	void EraseBuffer(int i);

	void SayConnections();
};