#include <iostream>
#include <SFML\Graphics.hpp>
#include <SFML\Network.hpp>
#include <string>
#include <cstring>
#include <vector>

#include "Funcs.h"
#include "ID.h"
#include "SocketManager.h"

#define MAX_MENSAJES 30





int main()
{
	std::vector<std::string> aMensajes;

	GameState gameState = GameState::USER_CONNECTION;

	
	SM.ServerInit();

	sf::Thread getClientMessage(&SocketManager::SocketReceive, &SM);
	getClientMessage.launch();

	struct Player {
		sf::Text name;
		std::vector<std::pair<Coordinate, int>> units;
		std::vector<std::pair<Coordinate, int>> base;
	};

	std::vector<Player> players;
	Player tempPl, tempPl2;
	tempPl.name.setString("\0");
	tempPl2.name.setString("\0");
	players.push_back(tempPl);
	players.push_back(tempPl2);


	int playerCounter = 0;
	bool playsA = true;
	bool done = false;
	while (!done) {
		switch (gameState)
		{
		case USER_CONNECTION: {
			for (int i = 0; i < players.size(); i++) {
				std::string t = &(*(SM.getBuffer(i)));
				if (t != "\0") {
					std::cout << "Players connected" << std::endl;
					SM.EraseBuffer(i);
					playerCounter++;
					if (playerCounter == 2) {
						gameState = GameState::NAME_INPUT;
						SM.SendMessage("Welcome");
					}
				}
			}
			break;
		}

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

		case TROOP_DEPLOY: {
			gameState = GameState::BASE_DEPLOY;
			std::string t;
			for (int i = 0; i < players.size(); i++) {
				t = &(*(SM.getBuffer(i)));
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
						players[i].units.push_back(std::make_pair(tCoord, STARTING_HP));
					}
				}
				//Comprobamos si se ha recibido las unidades de un jugador
				if (players[i].units.size() == 0) gameState = GameState::TROOP_DEPLOY;
			}

			if (gameState == GameState::BASE_DEPLOY) {
				std::string t = "UnitSetup_" + std::to_string(players[0].units.size());
				for (auto it : players[0].units) {
					t += "_" + std::to_string(it.first.first);
					t += "_" + std::to_string(it.first.second);
				}
				SM.SendMessage(t, 1);

				t = "UnitSetup_" + std::to_string(players[1].units.size());
				for (auto it : players[1].units) {
					t += "_" + std::to_string(it.first.first);
					t += "_" + std::to_string(it.first.second);
				}
				SM.SendMessage(t, 0);
			}
			break;
		}
		case BASE_DEPLOY: {
			gameState = GameState::START_GAME;
			std::string t;
			for (int i = 0; i < players.size(); i++) {
				t = &(*(SM.getBuffer(i)));
				if (t != "\0") {
					SM.EraseBuffer(i);
					t = t.substr(t.find('_') + 1, t.size());
					int bases = std::stoi(t.substr(0, t.find('_')));
					std::cout << "Received " << bases << " bases from player: " << i << std::endl;

					for (int j = 0; j < bases; j++) {
						Coordinate tCoord;
						t = t.substr(t.find('_') + 1, t.size());
						tCoord.first = stoi(t.substr(0, t.find('_')));
						t = t.substr(t.find('_') + 1, t.size());
						tCoord.second = stoi(t.substr(0, t.find('_')));

						std::cout << "Base " << j << ": " << tCoord.first << ", " << tCoord.second << std::endl;
						players[i].base.push_back(std::make_pair(tCoord, (int)STARTING_HP));
					}
				}
				//Comprobamos si se ha recibido las unidades de un jugador
				if (players[i].base.size() == 0) gameState = GameState::BASE_DEPLOY;
			}

			if (gameState == GameState::START_GAME) {
				t = "BaseSetup_" + std::to_string(players[0].base.size());
				for (auto it : players[0].base) {
					t += "_" + std::to_string(it.first.first);
					t += "_" + std::to_string(it.first.second);
				}
				SM.SendMessage(t, 1);

				t = "BaseSetup_" + std::to_string(players[1].base.size());
				for (auto it : players[1].base) {
					t += "_" + std::to_string(it.first.first);
					t += "_" + std::to_string(it.first.second);
				}
				SM.SendMessage(t, 0);
			}
			break;
		}

		/*case START_GAME:
			std::string t = "UnitSetup_" + std::to_string(players[1].units.size());
			for (auto it : players[1].units) {
				t += "_" + std::to_string(it.first.first);
				t += "_" + std::to_string(it.first.second);
				t += "_" + std::to_string(it.second);
			}
			t += "BaseSetup_" + std::to_string(players[1].base.size());
			for (auto it : players[1].base) {
				t += "_" + std::to_string(it.first.first);
				t += "_" + std::to_string(it.first.second);
				t += "_" + std::to_string(it.second);
			}
			SM.SendMessage(t, 0);
			gameState = GameState::GAME_LOOP;
			break;*/

		case GAME_LOOP:
			std::string t = &(*(SM.getBuffer(0)));
			if (t != "\0") {
				int i = playsA ? 0 : 1;
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
					players[i].units[j].first = tCoord;

					t = t.substr(t.find('_') + 1, t.size());
					players[i].units[j].second = stoi(t.substr(0, t.find('_')));
				}

				t = "UnitSetup_" + std::to_string(players[i].units.size());
				for (auto it : players[i].units) {
					t += "_" + std::to_string(it.first.first);
					t += "_" + std::to_string(it.first.second);
					t += "_" + std::to_string(it.second);
				}
				t += "BaseSetup_" + std::to_string(players[i].base.size());
				for (auto it : players[i].base) {
					t += "_" + std::to_string(it.first.first);
					t += "_" + std::to_string(it.first.second);
					t += "_" + std::to_string(it.second);
				}
				SM.SendMessage(t, !playsA ? 0 : 1);
				playsA != playsA;
			}
			break;
		//default:
			//break;
		}
	}
	getClientMessage.terminate();

	return 0;
}