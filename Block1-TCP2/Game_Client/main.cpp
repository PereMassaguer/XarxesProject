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
	sf::Vector2i screenDimensions(1080, 720);

	sf::RenderWindow window;
	window.create(sf::VideoMode(screenDimensions.x, screenDimensions.y), "Client");
	window.setPosition(sf::Vector2i(0, 200));

	sf::Font font;
	if (!font.loadFromFile("courbd.ttf")) std::cout << "Can't load the font file" << std::endl;

	WorldMap worldMap;
	//GUIButton _button(Coordinate(450, 600), Coordinate(300, 75));
	//GUIButton _button2(Coordinate(450, 450), Coordinate(300, 75));
	GUIButton _createGame("Create new game", Coordinate(675, 150), Coordinate(250, 75));
	GUIButton _refresh("Refresh", Coordinate(400, 175), Coordinate(129, 50));
	GUIButton _hideFull("Hide full", Coordinate(400, 125), Coordinate(129, 50));
	GUIButton _finishGame("Finish game", Coordinate(400, 350), Coordinate(200, 100));
	GUIButton _morePlayers("+", Coordinate(1010, 170), Coordinate(30, 30));
	GUIButton _lessPlayers("-", Coordinate(1010, 196), Coordinate(30, 30));

	_morePlayers.SetReady(true);
	_createGame.SetReady(true);
	_refresh.SetReady(true);
	_finishGame.SetReady(true);

	struct GameData {
		sf::String gameId;
		int currentPlayers;
		int maxPlayers;

		sf::Text gameName;
		sf::Text players;
		GUIButton button;
	};
	std::vector<GameData> _gameData;

	std::vector<sf::Text> chatList;
	int maxPlayers = 2;


	sf::String mensaje;
	mensaje = "";


	sf::Text text(mensaje, font, 20);
	text.setFillColor(sf::Color(0, 160, 0));
	text.setStyle(sf::Text::Bold);
	text.setPosition(750, 625 + 35);
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
	

	sf::Text lobby("Lobby", font, 60);
	lobby.setFillColor(sf::Color(255, 255, 255));
	lobby.setStyle(sf::Text::Bold);
	lobby.setPosition(50, 50);

	sf::Text gamelistT("Game list", font, 40);
	gamelistT.setFillColor(sf::Color(255, 255, 255));
	gamelistT.setStyle(sf::Text::Bold);
	gamelistT.setPosition(150, 175);

	sf::Text players("Players", font, 25);
	players.setFillColor(sf::Color(255, 255, 255));
	players.setStyle(sf::Text::Bold);
	players.setPosition(565, 285);

	sf::Text join("Join", font, 25);
	join.setFillColor(sf::Color(255, 255, 255));
	join.setStyle(sf::Text::Bold);
	join.setPosition(750, 285);

	sf::Text players2("Players", font, 20);
	players2.setFillColor(sf::Color(255, 255, 255));
	players2.setStyle(sf::Text::Bold);
	players2.setPosition(950, 140);

	sf::Text Nplayers(std::to_string(maxPlayers), font, 30);
	Nplayers.setFillColor(sf::Color(255, 255, 255));
	Nplayers.setStyle(sf::Text::Bold);
	Nplayers.setPosition(975, 180);



	texts.push_back(&gamelistT);
	texts.push_back(&lobby);
	texts.push_back(&players);
	texts.push_back(&join);
	texts.push_back(&players2);
	texts.push_back(&Nplayers);



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

				if (gameState == GameState::GAME_LOOP) {
					if (evento.key.code == sf::Keyboard::Return) {
						if (mensaje.getSize() > 0) {
							SM.SendMessage("MSG_" + mensaje);
							mensaje.clear();
						}
					}
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

					//_button.SetReady(mensaje.getSize() >= 2 ? true : false);
					user.name.setString(mensaje.getSize() == 0 ? "Write your name" : mensaje);
				}
				if (gameState == GameState::GAME_LOOP) {
					if (evento.text.unicode >= 32 && evento.text.unicode <= 126 && mensaje.getSize() < 10) {
						if (evento.text.unicode == 32) evento.text.unicode = 95;
						mensaje += (char)evento.text.unicode;
					}
					else if (evento.text.unicode == 8 && mensaje.getSize() > 0)
						mensaje.erase(mensaje.getSize() - 1, mensaje.getSize());
					text.setString(mensaje);
				}

				break;

			case sf::Event::MouseButtonPressed:

				/*if (_button2.CheckActivated(evento.mouseButton)) {
					SM.SendMessage("Exit");
				}*/

				if (gameState == GameState::NAME_INPUT) {
					if (_createGame.CheckActivated(evento.mouseButton)) {
						gameState = GameState::GAME_LOOP;
						SM.SendMessage("Create_" + std::to_string(maxPlayers));
					}

					if (_refresh.CheckActivated(evento.mouseButton)) {
						SM.SendMessage("Hello");
						_refresh.SetReady(true);
					}

					if (_morePlayers.CheckActivated(evento.mouseButton)) {
						maxPlayers++;
						if(maxPlayers < 4) _morePlayers.SetReady(true);
						if (maxPlayers > 2) _lessPlayers.SetReady(true);
						Nplayers.setString(std::to_string(maxPlayers));
					}
					
					if (_lessPlayers.CheckActivated(evento.mouseButton)) {
						maxPlayers--;
						if (maxPlayers < 4) _morePlayers.SetReady(true);
						if (maxPlayers > 2) _lessPlayers.SetReady(true);
						Nplayers.setString(std::to_string(maxPlayers));
					}

					for (auto it : _gameData) {
						if (it.button.CheckActivated(evento.mouseButton)) {
							SM.SendMessage("Join_" + it.gameId);
							break;
						}
					}
				}
				if (gameState == GameState::GAME_LOOP) {
					if (_finishGame.CheckActivated(evento.mouseButton)) {
						_createGame.SetReady(true);
						gameState = GameState::NAME_INPUT;
						_finishGame.SetReady(true);
						SM.SendMessage("FINISH");
						SM.SendMessage("Hello");
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

			if (t.substr(0, t.find("_")) == "GAMELIST") {
				_gameData.clear();

				t = t.substr(t.find("_") + 1, t.size());

				int gamesN = stoi(t.substr(0, t.find("_")));

				if (gamesN > 0) {
					t = t.substr(t.find("_"), t.size());

					for (int i = 0; i < gamesN; i++) {
						GameData tGD;
						t = t.substr(t.find("_") + 1, t.size());
						tGD.gameId = t.substr(0, t.find("_"));
						t = t.substr(t.find("_") + 1, t.size());
						tGD.currentPlayers = stoi(t.substr(0, t.find("_")));
						t = t.substr(t.find("_") + 1, t.size());
						tGD.maxPlayers = stoi(t.substr(0, t.find("_")));
						_gameData.push_back(tGD);
					}
					int i = -1;
					for (auto &it : _gameData) {		
						//if(_HideFullGame == true) continue;
						i++;
						it.gameName = sf::Text("lobby: " + it.gameId + "	  ", font, 30);						
						it.gameName.setFillColor(sf::Color(255, 255, 255));
						it.gameName.setStyle(sf::Text::Bold);
						it.gameName.setPosition(125, 325 + i * 35);

						it.players = sf::Text(std::to_string(it.currentPlayers) + "/" + std::to_string(it.maxPlayers), font, 30);
						it.players.setFillColor(sf::Color(255, 255, 255));
						it.players.setStyle(sf::Text::Bold);
						it.players.setPosition(600, 325 + i * 35);

						it.button = GUIButton(Coordinate(730, 325 + i * 35), Coordinate(100, 39));
						it.button.SetReady(true);
					}
				}
			}


			if (t.substr(0, t.find("_")) == "MSG") {
				if (chatList.size() < 5) {
					sf::Text tText("33", font, 20);
					tText.setFillColor(sf::Color(0, 160, 0));
					tText.setStyle(sf::Text::Bold);
					tText.setPosition(750, 625 - chatList.size() * 35);

					chatList.push_back(tText);
				}
				for (int i = chatList.size() - 1; i > 0; i--)
					chatList[i].setString(chatList[i - 1].getString());
				chatList[0].setString(t.substr(t.find("_") + 1, t.size()));
			}

			if (t.substr(0, t.find("_")) == "JOIN") {
				if(t.substr(t.find("_") + 1, t.size()) == "OK")
					gameState = GameState::GAME_LOOP;
				else if (t.substr(t.find("_") + 1, t.size()) == "FAILURE") {
					SM.SendMessage("Hello");
				}
			}

			if (t.substr(0, t.find("_")) == "Finish") {
				_createGame.SetReady(true);
				gameState = GameState::NAME_INPUT;
				_finishGame.SetReady(true);
				SM.SendMessage("Hello");
			}

			SM.EraseBuffer();
		}




		if (gameState == GameState::NAME_INPUT) {
			_refresh.Draw(window);
			_createGame.Draw(window);

			for (int i = 0; i < _gameData.size(); i++) {
				window.draw(_gameData[i].gameName);
				window.draw(_gameData[i].players);
				_gameData[i].button.Draw(window);
			}
			for (auto it : texts) window.draw(*it);
			_morePlayers.Draw(window);
			_lessPlayers.Draw(window);
		}
		if (gameState == GameState::GAME_LOOP) {
			window.draw(text);
			for (auto it : chatList)
				window.draw(it);
			_finishGame.Draw(window);
		}
		//worldMap.Draw(window);
		//_button.Draw(window);
		//_button2.Draw(window);
		//_HideFull.Draw(window);



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