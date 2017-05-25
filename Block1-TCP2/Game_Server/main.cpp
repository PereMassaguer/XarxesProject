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
	std::vector<Client> _clients;
	SM.ServerInit(&_clients);

	sf::Thread getClientMessage(&SocketManager::SocketReceive, &SM);
	getClientMessage.launch();


	Game game;


	bool done = false;
	int i = 0;
	while (!done) {
		std::string str;
		for (auto it : _clients) {
			str = it.buffer;

			if (str != "") {
				if (str == "Hello") {
					Debug(str);
				}


			}
		}
	}
	

	
	getClientMessage.terminate();

	return 0;
}