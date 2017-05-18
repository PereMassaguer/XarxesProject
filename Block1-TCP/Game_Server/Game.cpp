#include "Game.h"

Game::Game() {

	gameState = GameState::USER_CONNECTION;
	playerCounter = 0;
	playsA = true;
	done = false;
}




Game::~Game() {

}

void Game::Run()
{
	while (true) {
		for (auto it : players) {

		}
		if(&(*(SM.getBuffer(i))))
	}
}

void Game::ResolveCommand(std::string str)
{
	switch (gameState){
		case USER_CONNECTION: {
			for (int i = 0; i < players.size(); i++) {
				if (str != "\0") {
					Debug("Players connected");
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

		case NAME_INPUT: {
			//Set del nombre
			gameState = GameState::TROOP_DEPLOY;
			for (int i = 0; i < players.size(); i++) {
				if (str != "\0" && players[i].name.getString().toAnsiString() == "\0") {
					Debug("Received player: " + std::to_string(i) + " " + str);
					SM.EraseBuffer(i);
					players[i].name.setString(str.substr(str.find('_') + 1, str.size()));
				}
				//Comprobamos si se ha recibido el nombre de todos los jugadores
				if (players[i].name.getString().toAnsiString() == "\0") gameState = GameState::NAME_INPUT;
			}
			if (gameState == GameState::TROOP_DEPLOY) {
				SM.SendMessage(players[0].name.getString(), 1);
				SM.SendMessage(players[1].name.getString(), 0);
			}
			break;
		}

		case TROOP_DEPLOY: {
			gameState = GameState::BASE_DEPLOY;
			for (int i = 0; i < players.size(); i++) {
				SM.EraseBuffer(i);
				str = str.substr(str.find('_') + 1, str.size());
				int units = std::stoi(str.substr(0, str.find('_')));
				Debug("Received " + std::to_string(units) + " units from player: " + std::to_string(i));

				for (int j = 0; j < units; j++) {
					Coordinate tCoord;
					str = str.substr(str.find('_') + 1, str.size());
					StringToCoord(tCoord, str);

					std::cout << "Unit " << j << ": " << CoordToString(tCoord) << std::endl;
					players[i].units.push_back(std::make_pair(tCoord, STARTING_HP));
				}
				//Comprobamos si se ha recibido las unidades de un jugador
				if (players[i].units.size() == 0) gameState = GameState::TROOP_DEPLOY;
			}

			if (gameState == GameState::BASE_DEPLOY) {
				std::string t = "UnitSetup_" + std::to_string(players[0].units.size());
				for (auto it : players[0].units) CoordToString(it.first);
				SM.SendMessage(t, 1);

				t = "UnitSetup_" + std::to_string(players[1].units.size());
				for (auto it : players[1].units) CoordToString(it.first);
				SM.SendMessage(t, 0);
			}
			break;
		}

		case BASE_DEPLOY: {
			gameState = GameState::START_GAME;
			for (int i = 0; i < players.size(); i++) {
				if (str != "\0") {
					SM.EraseBuffer(i);
					str = str.substr(str.find('_') + 1, str.size());
					int bases = std::stoi(str.substr(0, str.find('_')));
					std::cout << "Received " << bases << " bases from player: " << i << std::endl;

					for (int j = 0; j < bases; j++) {
						Coordinate tCoord;
						str = str.substr(str.find('_') + 1, str.size());
						StringToCoord(tCoord, str);

						std::cout << "Base " << j << ": " << tCoord.first << ", " << tCoord.second << std::endl;
						players[i].base.push_back(std::make_pair(tCoord, (int)STARTING_HP));
					}
				}
				//Comprobamos si se ha recibido las unidades de un jugador
				if (players[i].base.size() == 0) gameState = GameState::BASE_DEPLOY;
			}

			if (gameState == GameState::START_GAME) {
				str = "BaseSetup_" + std::to_string(players[0].base.size());
				for (auto it : players[0].base) str += "_" + CoordToString(it.first);
				SM.SendMessage(str, 1);

				str = "BaseSetup_" + std::to_string(players[1].base.size());
				for (auto it : players[1].base) str += "_" + CoordToString(it.first);
				SM.SendMessage(str, 0);

				gameState = GameState::GAME_LOOP;
			}
			break;
		}

		case GAME_LOOP: {
			int i = playsA ? 0 : 1;
			SM.EraseBuffer(i);
			str = str.substr(str.find('_') + 1, str.size());
			int units = std::stoi(str.substr(0, str.find('_')));
			std::cout << "Received " << units << " units from player: " << i << std::endl;

			for (int j = 0; j < units; j++) {
				Coordinate tCoord;
				str = str.substr(str.find('_') + 1, str.size());
				StringToCoord(tCoord, str);

				std::cout << "Unit " << j << ": " << CoordToString(tCoord) << std::endl;
				players[i].units[j].first = tCoord;

				str = str.substr(str.find('_') + 1, str.size());
				players[i].units[j].second = stoi(str.substr(0, str.find('_')));
			}

			str = "UnitSetup_" + std::to_string(players[i].units.size());
			for (auto it : players[i].units) {
				str += "_" + CoordToString(it.first);
				str += "_" + std::to_string(it.second);
			}

			str += "BaseSetup_" + std::to_string(players[i].base.size());
			for (auto it : players[i].base) {
				str += "_" + CoordToString(it.first);
				str += "_" + std::to_string(it.second);
			}
			SM.SendMessage(str, !playsA ? 0 : 1);
			playsA != playsA;
			break;
		}
	}
}