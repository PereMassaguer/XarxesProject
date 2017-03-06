#include <iostream>
#include <SFML\Graphics.hpp>
#include <SFML\Network.hpp>
#include <string>
#include <cstring>
#include <vector>

#include "GraphicsManager.h"
#include "SocketManager.h"

#define MAX_MENSAJES 30


int main()
{
	///////////////////////////////////////////////////Graphics
	std::vector<std::string> User_messages;
	std::vector<std::string> Opponent_messages;


	sf::Vector2i screenDimensions(800, 600);

	sf::RenderWindow window;
	window.create(sf::VideoMode(screenDimensions.x, screenDimensions.y), "Client");

	sf::Font font;
	if (!font.loadFromFile("courbd.ttf")) std::cout << "Can't load the font file" << std::endl;


	sf::String mensaje;
	mensaje = " >";
	sf::Text chattingText(mensaje, font, 14);
	chattingText.setFillColor(sf::Color(0, 160, 0));
	chattingText.setStyle(sf::Text::Bold);


	sf::Text text(mensaje, font, 14);
	text.setFillColor(sf::Color(0, 160, 0));
	text.setStyle(sf::Text::Bold);
	text.setPosition(0, 560);

	std::vector<sf::RectangleShape> separator;
	std::vector<sf::Text*> texts;

	sf::RectangleShape t(sf::Vector2f(800, 5));
	t.setFillColor(sf::Color(200, 200, 200, 255));
	t.setPosition(0, 550);
	separator.push_back(t);

	t.setSize(sf::Vector2f(5, 525));
	t.setPosition(window.getSize().x / 2, 75);
	separator.push_back(t);

	t.setSize(sf::Vector2f(800, 5));
	t.setPosition(0, 75);
	separator.push_back(t);

	t.setSize(sf::Vector2f(800, 5));
	t.setPosition(0, 150);
	separator.push_back(t);

	t.setSize(sf::Vector2f(5, 75));
	t.setPosition(650, 0);
	separator.push_back(t);

	
	///////////////////////////////////////////////////Graphics\

	std::string targetWords[5] { "Word_1", "Word_2", "Word_3", "Word_4", "Word_5"};
	sf::Text targetWord(targetWords[0], font, 14);
	targetWord.setFillColor(sf::Color(255, 255, 0));
	targetWord.setStyle(sf::Text::Bold);
	targetWord.setPosition(30, 30);


	struct player {
		sf::Text name;
		sf::Text score;
	};
	player user;
	player opponent;

	user.name.setFillColor(sf::Color(255, 255, 255));
	user.name.setStyle(sf::Text::Bold);
	user.name.setPosition(30, 105);


	sf::Text auxText("Random_Name", font, 14);
	auxText.setFillColor(sf::Color(255, 255, 255));
	auxText.setStyle(sf::Text::Bold);
	auxText.setPosition(window.getSize().x / 2 + 30, 105);
	opponent.name = auxText;

	auxText.setString("User_Name");
	auxText.setPosition(30, 105);
	user.name = auxText;

	auxText.setString("0");
	auxText.setPosition(window.getSize().x / 2 - 50, 105);
	auxText.setFillColor(sf::Color(0, 255, 255));
	user.score = auxText;

	auxText.setPosition(window.getSize().x - 50, 105);
	opponent.score = auxText;


	sf::Text timeLeft("4321", font, 14);
	timeLeft.setFillColor(sf::Color(255, 0, 0));
	timeLeft.setStyle(sf::Text::Bold);
	timeLeft.setPosition(window.getSize().x - 125, 30);

	



	texts.push_back(&targetWord);
	texts.push_back(&timeLeft);
	texts.push_back(&user.name);
	texts.push_back(&user.score);
	texts.push_back(&opponent.name);
	texts.push_back(&opponent.score);


































	std::string responseText = "Connected to: Client";
	SM.ClientInit();

	sf::Thread getClientMessage(&SocketManager::SocketReceive, &SM);
	getClientMessage.launch();

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
					SM.SendMessage(mensaje);
					if (User_messages.size() > 25) User_messages.erase(User_messages.begin(), User_messages.begin() + 1);
					mensaje = " >";
				}
				break;
			case sf::Event::TextEntered:
				if (evento.text.unicode >= 32 && evento.text.unicode <= 126)
					mensaje += (char)evento.text.unicode;
				else if (evento.text.unicode == 8 && mensaje.getSize() > 2)
					mensaje.erase(mensaje.getSize() - 1, mensaje.getSize());
				break;
			}
		}
		if (*(SM.getBuffer()) != '\0') {
			//std::cout << *(SM.getBuffer()) << std::endl;
			User_messages.push_back(SM.getBuffer());
			SM.EraseBuffer();
		}

		for(auto it : separator) window.draw(it);

		for (size_t i = 0; i < User_messages.size(); i++) {
			std::string chatting = User_messages[i];
			chatting.erase(0, 2);
			chattingText.setPosition(sf::Vector2f(25, 175 + 20 * i));
			chattingText.setString(chatting);
			window.draw(chattingText);
		}
		std::string mensaje_ = mensaje + "_";
		text.setString(mensaje_);
		window.draw(text);
		for (auto it : texts) window.draw(*it);
		


		window.display();
		window.clear();

		if (SM.GetConnectionStatus() == sf::TcpSocket::Disconnected) {
			std::cout << "Server down" << std::endl;
			window.close();
		}
	}
	SM.Disconnect();
	getClientMessage.terminate();

	std::cout << "Connection stopped" << std::endl;
	//system("pause");
	return 0;
}