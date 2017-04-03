#include <iostream>
#include <SFML\Graphics.hpp>
#include <SFML\Network.hpp>
#include <string>
#include <cstring>
#include <vector>

#include "GraphicsManager.h"
#include "SocketManager.h"

int main()
{
	SM.ServerInit();

	sf::Thread getClientMessage(&SocketManager::SocketReceive, &SM);
	getClientMessage.launch();

	std::cout << "1 - Play music" << std::endl;
	std::cout << "2 - Stop music" << std::endl;
	std::cout << "3+N - Spawn N monsters" << std::endl;
	std::cout << "4 - Erase monsters" << std::endl;
	while (true) {
		sf::Event evento;
		std::string t;
		std::cin >> t;
		if(t == "1" || t == "2" || t[0] == '3' || t == "4")
			SM.SendMessage(t);
	}
	getClientMessage.terminate();

	return 0;
}