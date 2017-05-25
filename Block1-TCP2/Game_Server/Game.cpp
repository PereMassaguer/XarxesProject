#include "Game.h"

Game::Game() {
	_gameState = GameState::USER_CONNECTION;
	_playerCounter = 0;
	_playsA = true;
	_done = false;
}




Game::~Game() {

}

void Game::Run()
{
	while (true) {
		for (int i = 0; i < _players.size(); i++) {
			std::string str = &(*(SM.getBuffer(_players[i].id)));
			SM.EraseBuffer(_players[i].id);
			if (str != "\0") {
				switch (_gameState) {
					case USER_CONNECTION: {
						Debug("Players connected");
						//SM.EraseBuffer(_players[i].id);
						_playerCounter++;
						if (_playerCounter == 2) {
							_gameState = GameState::NAME_INPUT;
							SM.SendMessage("Welcome");
						}
						break;
					}

					case NAME_INPUT: {
						//Set del nombre
						_gameState = GameState::TROOP_DEPLOY;
						if (_players[i].name.getString().toAnsiString() == "\0") {
							Debug("Received player: " + std::to_string(i) + " " + str);
							//SM.EraseBuffer(i);
							_players[i].name.setString(str.substr(str.find('_') + 1, str.size()));
						}

						//Comprobamos si se ha recibido el nombre de todos los jugadores
						if (_players[i].name.getString().toAnsiString() == "\0") 
							_gameState = GameState::NAME_INPUT;

						if (_gameState == GameState::TROOP_DEPLOY) {
							SM.SendMessage(_players[0].name.getString(), _players[1].id);
							SM.SendMessage(_players[1].name.getString(), _players[0].id);
						}
						break;
					}

					case TROOP_DEPLOY: {
						_gameState = GameState::BASE_DEPLOY;
						//SM.EraseBuffer(i);
						str = str.substr(str.find('_') + 1, str.size());
						int units = std::stoi(str.substr(0, str.find('_')));
						Debug("Received " + std::to_string(units) + " units from player: " + std::to_string(i));

						for (int j = 0; j < units; j++) {
							Coordinate tCoord;
							str = str.substr(str.find('_') + 1, str.size());
							StringToCoord(tCoord, str);

							std::cout << "Unit " << j << ": " << CoordToString(tCoord) << std::endl;
							_players[i].units.push_back(std::make_pair(tCoord, STARTING_HP));
						}

						//Comprobamos si se ha recibido las unidades de un jugador
						if (_players[i].units.size() == 0) _gameState = GameState::TROOP_DEPLOY;

						if (_gameState == GameState::BASE_DEPLOY) {
							std::string t = "UnitSetup_" + std::to_string(_players[0].units.size());
							for (auto it : _players[0].units) CoordToString(it.first);
							SM.SendMessage(t, _players[1].id);

							t = "UnitSetup_" + std::to_string(_players[1].units.size());
							for (auto it : _players[1].units) CoordToString(it.first);
							SM.SendMessage(t, _players[0].id);
						}
						break;
					}

					case BASE_DEPLOY: {
						_gameState = GameState::START_GAME;

						//SM.EraseBuffer(i);
						str = str.substr(str.find('_') + 1, str.size());
						int bases = std::stoi(str.substr(0, str.find('_')));
						std::cout << "Received " << bases << " bases from player: " << i << std::endl;

						for (int j = 0; j < bases; j++) {
							Coordinate tCoord;
							str = str.substr(str.find('_') + 1, str.size());
							StringToCoord(tCoord, str);

							std::cout << "Base " << j << ": " << tCoord.first << ", " << tCoord.second << std::endl;
							_players[i].base.push_back(std::make_pair(tCoord, (int)STARTING_HP));
						}

						//Comprobamos si se ha recibido las unidades de un jugador
						if (_players[i].base.size() == 0) _gameState = GameState::BASE_DEPLOY;

						if (_gameState == GameState::START_GAME) {
							str = "BaseSetup_" + std::to_string(_players[0].base.size());
							for (auto it : _players[0].base) str += "_" + CoordToString(it.first);
							SM.SendMessage(str, _players[1].id);

							str = "BaseSetup_" + std::to_string(_players[1].base.size());
							for (auto it : _players[1].base) str += "_" + CoordToString(it.first);
							SM.SendMessage(str, _players[0].id);

							_gameState = GameState::GAME_LOOP;
						}
						break;
					}

					case GAME_LOOP: {
						int i = _playsA ? 0 : 1;
						//SM.EraseBuffer(i);
						str = str.substr(str.find('_') + 1, str.size());
						int units = std::stoi(str.substr(0, str.find('_')));
						std::cout << "Received " << units << " units from player: " << i << std::endl;

						for (int j = 0; j < units; j++) {
							Coordinate tCoord;
							str = str.substr(str.find('_') + 1, str.size());
							StringToCoord(tCoord, str);

							std::cout << "Unit " << j << ": " << CoordToString(tCoord) << std::endl;
							_players[i].units[j].first = tCoord;

							str = str.substr(str.find('_') + 1, str.size());
							_players[i].units[j].second = stoi(str.substr(0, str.find('_')));
						}

						str = "UnitSetup_" + std::to_string(_players[i].units.size());
						for (auto it : _players[i].units) {
							str += "_" + CoordToString(it.first);
							str += "_" + std::to_string(it.second);
						}

						str += "BaseSetup_" + std::to_string(_players[i].base.size());
						for (auto it : _players[i].base) {
							str += "_" + CoordToString(it.first);
							str += "_" + std::to_string(it.second);
						}
						SM.SendMessage(str, _players[!_playsA ? 0 : 1].id);
						_playsA = !_playsA;
						break;
					}
				}
			}
		}
	}
}

void Game::ConnectClient(Player & player)
{
	_players.push_back(player);
}

void Game::ResolveCommand(std::string str)
{
	
}