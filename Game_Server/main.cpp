#include <iostream>
#include <SFML\Graphics.hpp>
#include <SFML\Network.hpp>
#include <string>
#include <cstring>
#include <vector>

#include "ID.h"
#include "SocketManager.h"

#define MAX_MENSAJES 30


int main()
{
	std::vector<std::string> aMensajes;

	GameState gameState = GameState::NAME_INPUT;


	SM.ServerInit();

	sf::Thread getClientMessage(&SocketManager::SocketReceive, &SM);
	getClientMessage.launch();

	struct Player {
		sf::Text name;
		std::vector<Coordinate> units;
		std::vector<Coordinate> base;
	};

	std::vector<Player> players;
	Player tempPl, tempPl2;
	tempPl.name.setString("\0");
	tempPl2.name.setString("\0");
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
			gameState = GameState::TROOP_DEPLOY;
			for (int i = 0; i < players.size(); i++) {
				std::string t = &(*(SM.getBuffer(i)));
				if (t != "\0" && players[i].name.getString().toAnsiString() == "\0") {
					std::cout << "Received player: " << i << " "<< t << std::endl;
					SM.EraseBuffer(i);
					players[i].name.setString(t.substr(t.find('_') + 1, t.size()));
				}

				//Comprobamos si se ha recibido el nombre de todos los jugadores
				if (players[i].name.getString().toAnsiString() == "\0") gameState = GameState::NAME_INPUT;
			}

			
			if (gameState == GameState::TROOP_DEPLOY) {
				SM.SendMessage(players[0].name.getString(), 1);
				SM.SendMessage(players[1].name.getString(), 0);
			}

			break;

		case TROOP_DEPLOY:
			gameState = GameState::GAME_LOOP;
			for (int i = 0; i < players.size(); i++) {
				std::string t = &(*(SM.getBuffer(i)));
				if (t != "\0") {
					SM.EraseBuffer(i);
					t = t.substr(t.find('_') + 1, t.size());
					int units = std::stoi(t.substr(0, t.find('_')));
					std::cout << "Received " << units << " units from player: " << i << std::endl;
					
					for (int j = 0; j < units; j++) {
						Coordinate tCoord;
						t = t.substr(t.find('_') + 1, t.size());
						tCoord.first = stoi(t.substr(0, t.find('_')));
						t = t.substr(t.find('_') + 1, t.size());
						tCoord.second = stoi(t.substr(0, t.find('_')));

						std::cout << "Unit " << j << ": " << tCoord.first << ", " << tCoord.second << std::endl;
						players[i].units.push_back(Coordinate());
					}
				}
				//Comprobamos si se ha recibido las unidades de un jugador
				if (players[i].units.size() == 0) gameState = GameState::TROOP_DEPLOY;
			}
			break;

		case GAME_LOOP:
			//std::cout << "gameloop" << std::endl;
			break;
		default:
			break;
		}
	}
	getClientMessage.terminate();

	return 0;
}