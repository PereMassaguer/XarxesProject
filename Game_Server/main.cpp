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


	SM.ServerInit();

	sf::Thread getClientMessage(&SocketManager::SocketReceive, &SM);
	getClientMessage.launch();

	struct Player {
		sf::Text name;
		sf::Text score;
	};

	std::vector<std::pair<Player, bool>> players;//player_data-gotData?
	std::pair<Player, bool> tempPl, tempPl2;
	players.push_back(tempPl);
	players.push_back(tempPl2);

	bool done = false;
	while (!done) {
		switch (gameState)
		{
		case USER_CONNECTION:

			break;

		case NAME_INPUT:
			//Set del nombre
			for (int i = 0; i < 2; i++) {
				if (*(SM.getBuffer(i)) != '\0' && !players[i].second) {
					std::string t = &(*(SM.getBuffer(i)));
					//SM.EraseBuffer(i);
					t = t.substr(t.find('_') + 1, t.size());
					t = t.substr(t.find('_') + 1, t.size());
					std::cout << t << std::endl;
					players[i].first.name.setString(t.substr(0, t.find('_')));

					players[i].second = true;
				}
			}

			//Comprobamos si se ha recibido el nombre de todos los jugadores
			gameState = GameState::GAME_LOOP;
			for (auto it : players)  if (!it.second) gameState = GameState::NAME_INPUT;
			if (gameState == GameState::GAME_LOOP) SM.SendMessage();

			break;
		case GAME_LOOP:
			std::cout << "gameloop" << std::endl;
			break;
		default:
			break;
		}

	}
	getClientMessage.terminate();

	return 0;
}