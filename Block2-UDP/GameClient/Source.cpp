#pragma once
#include <PlayerInfo.h>
#include <SFML\Network.hpp>


#include "WorldMap.h"


int main()
{
	WorldMap worldMap;
	PlayerInfo playerInfo;


	sf::Vector2i screenDimensions(WINDOW_X, WINDOW_Y);
	sf::RenderWindow window;
	window.create(sf::VideoMode(screenDimensions.x, screenDimensions.y), "Client");
	window.setPosition(sf::Vector2i(0, 200));


	while (window.isOpen()) {
		worldMap.Draw(window);
		window.display();

		//window.clear();
	}
	return 0;
}