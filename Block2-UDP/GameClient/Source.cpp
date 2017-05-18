#pragma once
//#include <PlayerInfo.h>

#include "ConnectionManager.h"
#include "WorldMap.h"


int main()
{
	WorldMap worldMap;
	//PlayerInfo playerInfo;

	sf::Vector2i screenDimensions(WINDOW_X, WINDOW_Y);
	sf::RenderWindow window;
	window.create(sf::VideoMode(screenDimensions.x, screenDimensions.y), "Client");
	window.setPosition(sf::Vector2i(0, 200));


	sf::Font font;
	if (!font.loadFromFile("courbd.ttf")) std::cout << "Can't load the font file" << std::endl;

	sf::String name;
	sf::Text instructions("Enter your name", font);
	instructions.setPosition(100, 100);



	Coordinate gridDisplacement = worldMap.GetGridDisplacement();



	sf::Thread receiveMessage(&ConnectionManager::Recv, &CM);

	GameState gameState = GameState::NAME_INPUT;
	sf::String msg;
	while (window.isOpen()) {
		sf::Event event;

		while (window.pollEvent(event)) {
			switch (event.type) {
			case sf::Event::KeyPressed:
				if (event.key.code == sf::Keyboard::Escape) window.close();
				
				if (event.key.code == sf::Keyboard::Return) {
					if (gameState == GameState::NAME_INPUT && msg.getSize() >= 2) {
						CM.InitClient(name);
						receiveMessage.launch();
						instructions.setString("Waiting for connection confirmation");
						gameState = GAME_LOOP;
					}
				}
				if (gameState == GAME_LOOP) {
					if (event.key.code == sf::Keyboard::Up || event.key.code == sf::Keyboard::Down || event.key.code == sf::Keyboard::Left || event.key.code == sf::Keyboard::Right)
						CM.Move(int(event.key.code));
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

		sf::CircleShape t(CELL_SIZE_X * 0.375);
		switch (gameState)
		{
		case USER_CONNECTION:
			break;
		case NAME_INPUT:
			instructions.setString("Enter your name: " + name);
			break;
		case GAME_LOOP: {
			std::vector<std::vector<Coordinate>> pCoords = CM.GetPlayersCoords();

			t.setFillColor(sf::Color(C_BLUE));
			for (int i = 0; i < pCoords.size(); i++) {
				if (i > 0) t.setFillColor(sf::Color(C_RED));
				for (auto it : pCoords[i]) {
					t.setPosition((it.first + 0.5) * CELL_SIZE_X + gridDisplacement.first - t.getRadius(), (it.second + 0.5) * CELL_SIZE_Y + gridDisplacement.second - t.getRadius());
					window.draw(t);
				}
			}
		}
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