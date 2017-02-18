#pragma once
#include <SFML\Network.hpp>
#include <iostream>
#include <thread>

//#define SM SocketManager::Instance()
class SocketManager
{
	sf::IpAddress ip;
	char buffer[2000];
	sf::TcpSocket socketS, socketR;
	char mode;

public:
	SocketManager();
	~SocketManager();
	/*inline static SocketManager &Instance() {
		static SocketManager r;
		return r;
	}*/
	
	void ServerInit();
	void ClientInit();
	void SocketReceive();
	void SendMessage(std::string message);

	char* getBuffer();

	void Init(char m);
};