#pragma once
#include <SFML\Network.hpp>
#include <iostream>
#include <thread>
#include "ID.h"
#include "Game.h"

#define CONNECT_PORT 5000

#define SM SocketManager::Instance()
class SocketManager
{
	int idCounter;
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

	void ServerInit(std::vector<Client> *clients);
	void SocketReceive();
};