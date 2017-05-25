#include <iostream>
#include <SFML\Graphics.hpp>
#include <SFML\Network.hpp>
#include <string>
#include <vector>

#include "ID.h"
#include "SocketManager.h"

#define MAX_MENSAJES 30



#include "Game.h"

int main()
{
	sf::Thread getClientMessage(&SocketManager::SocketReceive, &SM);
	getClientMessage.launch();

	


	std::vector<Client> _clients;
	//SM.ServerInit(&_clients);
	int i = 1;
	while (i < 0) {
		i++;
	}

	getClientMessage.terminate();

	return 0;
}