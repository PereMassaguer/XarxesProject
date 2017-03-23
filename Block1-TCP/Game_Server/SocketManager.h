#pragma once
#include <SFML\Network.hpp>
#include <iostream>
#include <thread>

#define CONNECT_PORT 5000

#define SM SocketManager::Instance()
class SocketManager
{
	sf::IpAddress ip;
	char buffer[2][2000];

	sf::TcpListener listener;
	std::vector<sf::TcpSocket*> clientSockets;
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