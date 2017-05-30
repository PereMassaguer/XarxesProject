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
			if (cl->playing == true) break;

			str = cl->buffer;
			cl->EraseBuffer();

			if (str != "") {
				cl->playing = true;
				Debug(str);
				if (str == "Hello") {
					std::string send = "GAMELIST_";
					send += std::to_string(_games.size());
					for (auto it : _games) {
						send += "_" + std::to_string(it.first);
						send += "_" + std::to_string(it.second.players);
					}
					SM.SendMessage(send, cl->socket);
				}
				if (str == "Create") {
					_games.insert(std::make_pair(_gameIdCounter++, Game()));

					std::map<int, Game>::iterator it;
					it = _games.find(_gameIdCounter);
					it->second.AddPlayer(&cl->player);
					cl->playing = true;

					SM.SendMessage("Create_OK", cl->socket);

					//it->second.RunCommand(cl.player.id, str);
				}
				if (str == "Exit") {

				}
			}
		}


		/*if (_clients.size() > 0) {
			i++;
		}
		for (auto it : _clients) {
			if (it.playing == true) break;

			str = it.buffer;

			if (str != "") {
				if (str == "Hello") {
					Debug(str);
					_games.AddClient(it);
				}

			}
		}*/
	}

	getClientMessage.terminate();

	return 0;
}