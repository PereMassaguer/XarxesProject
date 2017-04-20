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
		if (lastConCheck < GetTickCount() - PING) {
			lastConCheck = GetTickCount();
			CM.Send("ACK");
		}
	}

	return 0;
}