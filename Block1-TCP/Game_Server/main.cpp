#include <iostream>
#include <SFML\Graphics.hpp>
#include <SFML\Network.hpp>
#include <string>
#include <vector>

#include "ID.h"
#include "SocketManager.h"

#define MAX_MENSAJES 30





int main()
{
	std::vector<std::string> aMensajes;

	
	SM.ServerInit();

	sf::Thread getClientMessage(&SocketManager::SocketReceive, &SM);
	getClientMessage.launch();

	

	std::vector<Player> players;
	Player tempPl, tempPl2;
	tempPl.name.setString("\0");
	tempPl2.name.setString("\0");
	players.push_back(tempPl);
	players.push_back(tempPl2);


	
	getClientMessage.terminate();

	return 0;
}