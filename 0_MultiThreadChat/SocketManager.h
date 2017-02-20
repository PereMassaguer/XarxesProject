#pragma once
#include <SFML\Network.hpp>
#include <iostream>
#include <thread>

class SocketManager
{
	sf::IpAddress ip;
	char buffer[2000];
	sf::TcpSocket socketS, socketR;
	char mode;

public:
	SocketManager();
	~SocketManager();
	
	void ServerInit();
	void ClientInit();
	void SocketReceive();
	void SendMessage(std::string message);

	char* getBuffer();
	void EraseBuffer();

	void Init(char m);
	void Disconnect();
};