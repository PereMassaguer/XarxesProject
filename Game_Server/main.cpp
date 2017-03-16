#include <iostream>
#include <SFML\Graphics.hpp>
#include <SFML\Network.hpp>
#include <string>
#include <cstring>
#include <vector>

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
	tempPl.first.name.setString("\0");
	tempPl2.first.name.setString("\0");
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
					std::cout << "Received player: " << i << " "<< &*(SM.getBuffer(i)) << std::endl;
					SM.EraseBuffer(i);
					t = t.substr(t.find('_') + 1, t.size());

					players[i].first.name.setString(t.substr(0, t.find('_')));

					players[i].second = true;
				}
			}

			//Comprobamos si se ha recibido el nombre de todos los jugadores
			gameState = GameState::GAME_LOOP;
			for (auto it : players) {
				if (it.first.name.getString().toAnsiString() == "\0") {
					gameState = GameState::NAME_INPUT;
				}
			}
			
			if (gameState == GameState::GAME_LOOP) {
				SM.SendMessage(players[0].first.name.getString(), 1);
				SM.SendMessage(players[1].first.name.getString(), 0);
			}

			break;
		case GAME_LOOP:
			//std::cout << "gameloop" << std::endl;
			break;
		default:
			break;
		}
		
		
		if (*(SM.getBuffer(0)) != '\0') {
			//std::cout << &*(SM.getBuffer(0)) << std::endl;
			//SM.EraseBuffer();
		}
	}
	getClientMessage.terminate();

	return 0;
}