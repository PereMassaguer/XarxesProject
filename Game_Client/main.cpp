#include <iostream>
#include <SFML\Network.hpp>
#include <string>
#include <cstring>
#include <vector>

#include "Cell.h"
#include "SocketManager.h"
#include "Unit.h"
#include "WorldMap.h"



int main()
{
	///////////////////////////////////////////////////Graphics
	sf::Vector2i screenDimensions(900, 675);

	sf::RenderWindow window;
	window.create(sf::VideoMode(screenDimensions.x, screenDimensions.y), "Client");

	WorldMap worldMap;
	GUIButton _button(Coordinate(450, 600));


	sf::Font font;
	if (!font.loadFromFile("courbd.ttf")) std::cout << "Can't load the font file" << std::endl;


	sf::String mensaje;
	mensaje = "";


	sf::Text text(mensaje, font, 14);
	text.setFillColor(sf::Color(0, 160, 0));
	text.setStyle(sf::Text::Bold);
	text.setPosition(0, 560);
	std::vector<sf::Text*> texts;


	///////////////////////////////////////////////////Graphics\
	
	struct player {
		sf::Text name;
		sf::Text score;
	};
	player user;
	player opponent;

	user.name.setFillColor(sf::Color(255, 255, 255));
	user.name.setStyle(sf::Text::Bold);
	user.name.setPosition(30, 105);


	sf::Text auxText("Connecting...", font, 24);
	auxText.setFillColor(sf::Color(255, 255, 255));
	auxText.setStyle(sf::Text::Bold);
	auxText.setPosition(window.getSize().x / 2 + 175, window.getSize().y - 55);
	opponent.name = auxText;

	auxText.setString("Write your name");
	auxText.setPosition(25, window.getSize().y - 55);
	user.name = auxText;

	auxText.setString("0");
	auxText.setPosition(window.getSize().x / 2 - 50, 105);
	auxText.setFillColor(sf::Color(0, 255, 255));
	user.score = auxText;

	auxText.setPosition(window.getSize().x - 50, 105);
	opponent.score = auxText;



	texts.push_back(&user.name);
	//texts.push_back(&user.score);
	texts.push_back(&opponent.name);
	//texts.push_back(&opponent.score);
	

	std::string responseText = "Connected to: Client";
	SM.ClientInit();

	sf::Thread getClientMessage(&SocketManager::SocketReceive, &SM);
	getClientMessage.launch();
	

	window.setPosition(sf::Vector2i(0, 200));
	
	GameState gameState = GameState::NAME_INPUT;

	while (window.isOpen()) {
		sf::Event evento;
		while (window.pollEvent(evento))
		{
			switch (evento.type)
			{
			case sf::Event::Closed:
				window.close();

				break;

			case sf::Event::KeyPressed:
				if (evento.key.code == sf::Keyboard::Escape)
					window.close();
				else if (evento.key.code == sf::Keyboard::Return) {
				}

				break;

			case sf::Event::TextEntered:
				if (gameState == GameState::NAME_INPUT) {
					if (evento.text.unicode >= 32 && evento.text.unicode <= 126 && mensaje.getSize() < 10) {
						if (evento.text.unicode == 32) evento.text.unicode = 95;
						mensaje += (char)evento.text.unicode;
					}
					else if (evento.text.unicode == 8 && mensaje.getSize() > 0)
						mensaje.erase(mensaje.getSize() - 1, mensaje.getSize());

					_button.SetReady(mensaje.getSize() >= 2 ? true : false);
					user.name.setString(mensaje.getSize() == 0 ? "Write your name" : mensaje);
				}
				break;

			case sf::Event::MouseButtonPressed:
				if (gameState == GameState::TROOP_DEPLOY) {
					worldMap.ActivateCell(evento.mouseButton, gameState);
					_button.SetReady(worldMap.GetPlayerUnits() == 3 ? true : false);
					if (_button.CheckActivated(evento.mouseButton)) {
						_button.SetReady(false);
						std::vector<Coordinate> tCoord = worldMap.GetPlayerUnitsCoords();
						std::string t = "Setup_" + std::to_string(tCoord.size());
						for (auto it : tCoord) {
							t += "_" + std::to_string(it.first);
							t += "_" + std::to_string(it.second);
						}
						SM.SendMessage(t);
					}
				}

				if (gameState == GameState::NAME_INPUT) {
					if (_button.CheckActivated(evento.mouseButton)) {
						_button.SetReady(false);
						std::string t = "Start_";
						t += user.name.getString();
						SM.SendMessage(t);
					}
				}				
				
				if (gameState == GameState::GAME_LOOP) {
					worldMap.ActivateCell(evento.mouseButton, gameState);
				}

				break;

			case sf::Event::MouseMoved:
				
				break;
			}	
		}

		if (*(SM.getBuffer()) != '\0') {
			if (gameState == GameState::NAME_INPUT) {
				std::string t = &(*(SM.getBuffer()));
				std::cout << t << std::endl;
				SM.EraseBuffer();
				t = t.substr(t.find('_') + 1, t.size());
				opponent.name.setString(t);
				gameState = GameState::TROOP_DEPLOY;
			}
		}

		worldMap.Draw(window);
		for (auto it : texts) window.draw(*it);
		_button.Draw(window);


		window.display();

		if (SM.GetConnectionStatus() == sf::TcpSocket::Disconnected) {
			std::cout << "Server down" << std::endl;
			window.close();
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(1000/144));
		window.clear();
	}
	SM.Disconnect();
	//getClientMessage.terminate();

	std::cout << "Connection stopped" << std::endl;
	//system("pause");
	return 0;
	
}