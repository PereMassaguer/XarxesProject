#include <iostream>
#include <SFML\Graphics.hpp>
#include <SFML\Network.hpp>
#include <string>
#include <cstring>
#include <vector>

#include "GraphicsManager.h"
#include "SocketManager.h"

#define MAX_MENSAJES 30


int main()
{
	std::vector<std::string> aMensajes;

	enum GameState {
		USER_CONNECTION,
		NAME_INPUT,
		GAME_LOOP
	};
	GameState gameState = GameState::NAME_INPUT;


	sf::String mensaje;
	mensaje = " >";


	std::string responseText = "Connected to: Server";
	SM.ServerInit();

	sf::Thread getClientMessage(&SocketManager::SocketReceive, &SM);
	getClientMessage.launch();

	struct player {
		sf::Text name;
		sf::Text score;
	};

	std::pair<player, bool> player_1, player_2;//player_data-gotData?


	bool done = false;
	while (!done) {
		switch (gameState)
		{
		case USER_CONNECTION:
			if (!player_1.second) {

			}
			break;
			
		case NAME_INPUT:

			break;
		case GAME_LOOP:
			break;
		default:
			break;
		}

	}
	getClientMessage.terminate();

	return 0;
}