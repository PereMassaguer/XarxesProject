#include <iostream>
#include <SFML\Graphics.hpp>
#include <SFML\Network.hpp>

#include <map>
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

	
	std::map<int, Game> _games;
	int _gameIdCounter = 0;


	bool done = false;
	int i = 0;

	while (!done) {
		Client* cl;
		for(int i = 0; i < _clients.size() ;i++){
			std::string str;
			cl = &_clients[i];

			str = cl->buffer;

			if (str != "") {
				Debug(str);
				if (str == "Hello") {
					std::string send = "GAMELIST_";
					send += std::to_string(_games.size());
					for (auto it : _games) {
						send += "_" + std::to_string(it.first);
						send += "_" + std::to_string(it.second.players);
						send += "_" + std::to_string(it.second.maxPlayers);
					}
					SM.SendMessage(send, cl->socket);
				}
				if (str.substr(0, str.find("_")) == "Create") {
					_games.insert(std::make_pair(_gameIdCounter, Game(stoi(str.substr(str.find("_") + 1, str.size())))));

					std::map<int, Game>::iterator it;
					it = _games.find(_gameIdCounter);
					it->second.AddPlayer(&cl->player);
					cl->gameID = _gameIdCounter;


					SM.SendMessage("Create_OK", cl->socket);

					_gameIdCounter++;
				}
				if (str.substr(0, str.find("_")) == "MSG") {
					for (auto it : _clients) {
						if(it.gameID == cl->gameID)
							SM.SendMessage("MSG_" + cl->player.name.getString() + ": " + str.substr(str.find("_") + 1, str.size()), it.socket);
					}					
				}

				if (str == "FINISH") {
					for (auto it : _clients) {
						if (it.gameID == cl->gameID && cl->player.id != it.player.id)
							SM.SendMessage("Finish", it.socket);
						it.gameID = -1;
					}
					_games.erase(cl->gameID);
				}

				if (str.substr(0, str.find("_")) == "Join") {
					int gameID = stoi(str.substr(str.find("_") + 1, str.size()));
					std::map<int, Game>::iterator it = _games.find(gameID);
					
					if (it->second.players < it->second.maxPlayers) {
						cl->gameID = gameID;
						it->second.AddPlayer(&cl->player);
						SM.SendMessage("JOIN_OK", cl->socket);
					}
					else {
						SM.SendMessage("JOIN_FAIL", cl->socket);
					}
				}
				cl->EraseBuffer();
			}
		}
	}

	getClientMessage.terminate();

	return 0;
}