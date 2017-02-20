#pragma once
#include <SFML\Network.hpp>
#include <iostream>
#include <thread>

#define CONNECT_PORT 5000

#define SM SocketManager::Instance()
class SocketManager
{
	sf::IpAddress ip;
	char buffer[2000];
	sf::TcpSocket socket;
	char mode;

public:
	SocketManager();
	~SocketManager();

	inline static SocketManager &Instance() {
		static SocketManager socketManager;
		return socketManager;
	}

	void ServerInit();
	void ClientInit();
	void SocketReceive();
	void SendMessage(std::string message);

	char* getBuffer();
	void EraseBuffer();

	void Init(char m);
	void Disconnect();
};