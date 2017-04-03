#pragma once
#include "ConnectionManager.h"
#include <SFML\Network.hpp>
#include <PlayerInfo.h>
#include <time.h>

#include <Windows.h>

int main()
{		
	struct Player {
		Coordinate coord;
		sf::IpAddress ip;
		unsigned short port;
	};


	int lastConCheck;
	PlayerInfo playerInfo;
	std::vector<Player> _players;


	CM.Init('s');

	sf::Thread receiveMessage(&ConnectionManager::Recv, &CM);
	receiveMessage.launch();

	lastConCheck = GetTickCount();

	while (true) {
		std::string message = CM.GetBuffer();
		if (message != "") {
			if (message.substr(0, message.find("_")) == "HELLO") {
				CM.EraseBuffer();
				CM.Send("WELCOME_0");
			}
		}

		else if (lastConCheck < GetTickCount() - STATUS_TIME) {
			lastConCheck = GetTickCount();
			CM.Send("CHECK");
		}
	}

	return 0;
}