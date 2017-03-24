#pragma once
#include "ConnectionManager.h"
#include <SFML\Network.hpp>
#include <PlayerInfo.h>

int main()
{	
	PlayerInfo playerInfo;


	CM.Init('s');

	sf::Thread receiveMessage(&ConnectionManager::Recv, &CM);
	receiveMessage.launch();


	while (true) {
		if (CM.GetBuffer() != "") {
			Debug(CM.GetBuffer());
		}
	}


	return 0;
}