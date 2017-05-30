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
	window.setPosition(sf::Vector2i(0, 200));

	sf::Font font;
	if (!font.loadFromFile("courbd.ttf")) std::cout << "Can't load the font file" << std::endl;

	WorldMap worldMap;
	GUIButton _button(Coordinate(450, 600));
	GUIButton _button2(Coordinate(450, 450));
	GUIButton _CreateGame(Coordinate(650, 125));
	_CreateGame.SetReady(true);

	sf::Text createGameT("Create new game", font, 24);
	createGameT.setFillColor(sf::Color(255, 255, 255));
	createGameT.setStyle(sf::Text::Bold);
	createGameT.setPosition(525, 150);





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
	auxText.setPosition(window.getSize().x - 250, 50);
	user.name = auxText;

	auxText.setString("0");
	auxText.setPosition(window.getSize().x / 2 - 50, 105);
	auxText.setFillColor(sf::Color(0, 255, 255));
	user.score = auxText;

	auxText.setPosition(window.getSize().x - 50, 105);
	opponent.score = auxText;


	texts.push_back(&user.name);
	//texts.push_back(&opponent.name);
	

	sf::Text lobby("Lobby", font, 40);
	lobby.setFillColor(sf::Color(255, 255, 255));
	lobby.setStyle(sf::Text::Bold);
	lobby.setPosition(100, 50);


	sf::Text gamelistT("Game list", font, 30);
	gamelistT.setFillColor(sf::Color(255, 255, 255));
	gamelistT.setStyle(sf::Text::Bold);
	gamelistT.setPosition(125, 150);



	texts.push_back(&gamelistT);
	texts.push_back(&lobby);
	texts.push_back(&createGameT);

	std::vector<sf::Text> gameList;



	SM.ClientInit();

	sf::Thread getClientMessage(&SocketManager::SocketReceive, &SM);
	getClientMessage.launch();
	

	GameState gameState = GameState::NAME_INPUT;
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

				if (_button2.CheckActivated(evento.mouseButton)) {
					SM.SendMessage("Exit");
				}
				if (_CreateGame.CheckActivated(evento.mouseButton)) {
					SM.SendMessage("Create");
				}

				break;

			case sf::Event::MouseMoved:
				
				break;
			}	
		}


		std::string t = &(*(SM.getBuffer()));
		if (*(SM.getBuffer()) != '\0') {
			std::cout << t << std::endl;

			if (t.substr(0, t.find("_")) == "GAMELIST") {
				t = t.substr(t.find("_") + 1, t.size());

				int gamesN = stoi(t.substr(0, t.find("_")));

				if (gamesN > 0) {
					t = t.substr(t.find("_"), t.size());

					for (int i = 0; i < gamesN; i++) {
						t = t.substr(t.find("_") + 1, t.size());
						sf::String lobbyId = t.substr(0, t.find("_"));
						t = t.substr(t.find("_") + 1, t.size());
						sf::String playerN = t.substr(0, t.find("_"));

						sf::Text text("Lobby Number id: " + lobbyId + "		" + playerN + "/2", font, 30);

						text.setFillColor(sf::Color(255, 255, 255));
						text.setStyle(sf::Text::Bold);
						text.setPosition(125, 200 + i * 35);

						gameList.push_back(text);
					}
				}
			}


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
			default:
				break;
			}
			SM.EraseBuffer();
		}

		//worldMap.Draw(window);
		//_button.Draw(window);
		_button2.Draw(window);
		_CreateGame.Draw(window);
		for (auto it : texts) window.draw(*it);


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