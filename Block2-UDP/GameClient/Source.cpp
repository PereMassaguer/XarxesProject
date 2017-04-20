#pragma once
#include <PlayerInfo.h>
#include <SFML\Network.hpp>

#include "ConnectionManager.h"
#include "WorldMap.h"


int main()
{
	WorldMap worldMap;
	PlayerInfo playerInfo;

	sf::Vector2i screenDimensions(WINDOW_X, WINDOW_Y);
	sf::RenderWindow window;
	window.create(sf::VideoMode(screenDimensions.x, screenDimensions.y), "Client");
	window.setPosition(sf::Vector2i(0, 200));


	sf::Font font;
	if (!font.loadFromFile("courbd.ttf")) std::cout << "Can't load the font file" << std::endl;

	sf::String name;
	sf::Text instructions("Enter your name", font);
	instructions.setPosition(100, 100);


	CM.Init('c');

	sf::Thread receiveMessage(&ConnectionManager::Recv, &CM);
	receiveMessage.launch();

	GameState gameState = GameState::NAME_INPUT;
	sf::String msg;
	bool waiting = false;
	while (window.isOpen()) {
		sf::Event event;

		while (window.pollEvent(event)) {
			switch (event.type) {
			case sf::Event::KeyPressed:
				if (event.key.code == sf::Keyboard::Escape) window.close();
				
				if (event.key.code == sf::Keyboard::Return) {
					if (gameState == GameState::NAME_INPUT && msg.getSize() >= 2 && !waiting) {
						CM.Send("HELLO_" + name);
						instructions.setString("Waiting for connection confirmation");
						waiting = true;
					}
				}
				break;

			case sf::Event::TextEntered:
				if (gameState == GameState::NAME_INPUT) {
					if (event.text.unicode >= 32 && event.text.unicode <= 126 && msg.getSize() < 10) {
						if (event.text.unicode == 32) event.text.unicode = 95;
						msg += (char)event.text.unicode;
					}
					else if (event.text.unicode == 8 && msg.getSize() > 0)
						msg.erase(msg.getSize() - 1, msg.getSize());
					name = msg;
				}
				break;

			default:
				break;
			}
		}

		switch (gameState)
		{
		case USER_CONNECTION:
			break;
		case NAME_INPUT:
			if(!waiting) instructions.setString("Enter your name: " + name);
			else {
				if (CM.GetBuffer() != "") {
					//Debug(CM.GetBuffer());
				}
			}
			break;
		case TROOP_DEPLOY:
			break;
		case BASE_DEPLOY:
			break;
		case GAME_LOOP:
			break;
		default:
			break;
		}


		worldMap.Draw(window);
		window.draw(instructions);
		window.display();

		window.clear();
	}
	return 0;
}