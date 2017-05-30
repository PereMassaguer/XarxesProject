#include <iostream>
#include <SFML\Graphics.hpp>
#include <SFML\Network.hpp>
#include <string>
#include <cstring>
#include <vector>

#include "SocketManager.h"



int main()
{
	///////////////////////////////////////////////////Graphics
	sf::Vector2i screenDimensions(900, 675);

	sf::RenderWindow window;
	window.create(sf::VideoMode(screenDimensions.x, screenDimensions.y), "Client");
	window.setPosition(sf::Vector2i(0, 200));

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
	texts.push_back(&opponent.name);
	
	SM.ClientInit();

	sf::Thread getClientMessage(&SocketManager::SocketReceive, &SM);
	getClientMessage.launch();
	

	GameState gameState = GameState::USER_CONNECTION;
	SM.SendMessage("Hello");


	bool waiting = false;
	while (window.isOpen()) {
		sf::Event evento;
		while (window.pollEvent(evento) && !waiting)
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

				if (gameState == GameState::NAME_INPUT) {
					if (_button.CheckActivated(evento.mouseButton)) {
						_button.SetReady(false);
						std::string t = "Start_";
						t += user.name.getString();
						SM.SendMessage(t);
						waiting = true;
					}
				}

				if (gameState == GameState::TROOP_DEPLOY) {
					worldMap.ActivateCell(evento.mouseButton, gameState);
					_button.SetReady(worldMap.GetPlayerUnits() == 3 ? true : false);
					if (_button.CheckActivated(evento.mouseButton)) {
						_button.SetReady(false);
						std::vector<Coordinate> tCoord = worldMap.GetPlayerUnitsCoords();
						std::string t = "UnitSetup_" + std::to_string(tCoord.size());

						for (auto it : tCoord) t += "_" + CoordToString(it);

						SM.SendMessage(t);
						waiting = true;
					}
				}				

				if (gameState == GameState::BASE_DEPLOY) {
					worldMap.ActivateCell(evento.mouseButton, gameState);
					_button.SetReady(worldMap.GetPlayerBases() == 2 ? true : false);
					if (_button.CheckActivated(evento.mouseButton)) {
						_button.SetReady(false);
						std::vector<Coordinate> tCoord = worldMap.GetPlayerBasesCoords();
						std::string t = "BaseSetup_" + std::to_string(tCoord.size());

						for (auto it : tCoord) t += "_" + CoordToString(it);

						SM.SendMessage(t);
						waiting = true;
					}
				}

				if (gameState == GameState::GAME_LOOP && !waiting) {
					worldMap.ActivateCell(evento.mouseButton, gameState);
					_button.SetReady(true);
					if (_button.CheckActivated(evento.mouseButton)) {
						_button.SetReady(false);
						std::vector<Coordinate> tCoord = worldMap.GetPlayerUnitsCoords();
						std::string t = "UnitSetup_" + std::to_string(tCoord.size());
						
						for (auto it : tCoord) t += "_" + CoordToString(it);

						tCoord = worldMap.GetPlayerBasesCoords();
						t += "BaseSetup_" + std::to_string(tCoord.size());

						for (auto it : tCoord) t += "_" + CoordToString(it);

						SM.SendMessage(t);
						waiting = true;
						worldMap.SetMovementsLeft(PLAYER_UNIT);
					}
				}

				break;

			case sf::Event::MouseMoved:
				
				break;
			}	
		}


		std::string t = &(*(SM.getBuffer()));
		if (*(SM.getBuffer()) != '\0') {
			std::cout << t << std::endl;
			switch (gameState)
			{
			case USER_CONNECTION:
				gameState = GameState::NAME_INPUT;
				break;
			case NAME_INPUT:
				t = t.substr(t.find('_') + 1, t.size());
				opponent.name.setString(t);
				gameState = GameState::TROOP_DEPLOY;
				waiting = false;
				break;
			case TROOP_DEPLOY:
				worldMap.SetEnemyUnits(t);
				gameState = GameState::BASE_DEPLOY;
				waiting = false;
				break;
			case BASE_DEPLOY:
				worldMap.SetEnemyBases(t);
				gameState = GameState::GAME_LOOP;
				break;
			case GAME_LOOP:
				worldMap.SetEnemyData(t);
				waiting = false;
				break;
			default:
				break;
			}
			SM.EraseBuffer();
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