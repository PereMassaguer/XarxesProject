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
	std::vector<std::string> aMensajes;

	sf::Vector2i screenDimensions(800, 600);

	sf::RenderWindow window;
	sf::String mensaje;
	mensaje = " >";
	window.create(sf::VideoMode(screenDimensions.x, screenDimensions.y), "Client");

	sf::Font font;
	if (!font.loadFromFile("courbd.ttf")) std::cout << "Can't load the font file" << std::endl;


	sf::Text chattingText(mensaje, font, 14);
	chattingText.setFillColor(sf::Color(0, 160, 0));
	chattingText.setStyle(sf::Text::Bold);


	sf::Text text(mensaje, font, 14);
	text.setFillColor(sf::Color(0, 160, 0));
	text.setStyle(sf::Text::Bold);
	text.setPosition(0, 560);

	sf::RectangleShape separator(sf::Vector2f(800, 5));
	separator.setFillColor(sf::Color(200, 200, 200, 255));
	separator.setPosition(0, 550);


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
					mensaje = "Client says:" + mensaje;
					SM.SendMessage(mensaje);
					if (aMensajes.size() > 25) aMensajes.erase(aMensajes.begin(), aMensajes.begin() + 1);
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
			aMensajes.push_back(SM.getBuffer());
			SM.EraseBuffer();
		}


		window.draw(separator);
		for (size_t i = 0; i < aMensajes.size(); i++) {
			std::string chatting = aMensajes[i];
			chattingText.setPosition(sf::Vector2f(0, 20 * i));
			chattingText.setString(chatting);
			window.draw(chattingText);
		}
		std::string mensaje_ = mensaje + "_";
		text.setString(mensaje_);
		window.draw(text);


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
	system("pause");
	return 0;
}