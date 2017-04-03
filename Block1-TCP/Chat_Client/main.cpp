#include <iostream>
#include <SFML\Graphics.hpp>
#include <SFML\Network.hpp>
#include <SFML\Audio.hpp>
#include <string>
#include <cstring>
#include <vector>

#include "GraphicsManager.h"
#include "SocketManager.h"

#define MAX_MENSAJES 30


int main()
{
	sf::Vector2i screenDimensions(800, 600);

	sf::RenderWindow window;
	window.create(sf::VideoMode(screenDimensions.x, screenDimensions.y), "Client");

	SM.ClientInit();

	sf::Thread getClientMessage(&SocketManager::SocketReceive, &SM);
	getClientMessage.launch();

	//Sound
	sf::SoundBuffer soundbuffer;
	if (!soundbuffer.loadFromFile("terror.wav")) {
		std::cout << "error music" << std::endl;
	}
	sf::Sound sound;
	sound.setBuffer(soundbuffer);
	sound.setLoop(true);

	//Image
	sf::Texture texture;
	texture.loadFromFile("monster.png");

	std::vector<sf::Sprite> monsters;


	while (window.isOpen()) {

		if (*(SM.getBuffer()) != '\0') {
			std::string t = SM.getBuffer();
			std::cout << t << std::endl;

			int i = stoi(t);

			switch (i)
			{
			case 1:
				sound.play();
				break;
			case 2:
				sound.stop();
				break;
			case 4:
				monsters.clear();
				break;
			default:
				std::string temp = t.substr(0, 1);
				if (stoi(temp) == 3) {
					monsters.clear();

					int nMonsters = stoi(t.substr(1, t.size()));
					sf::Sprite sprite;
					sprite.setTexture(texture, true);
					for (int i = 0; i < nMonsters; i++) {
						float x = ((double)rand() / (RAND_MAX + 1)) * 800;
						float y = ((double)rand() / (RAND_MAX + 1)) * 600;
						sprite.setPosition(x, y);
						monsters.push_back(sprite);
					}
				}

				break;
			}
			SM.EraseBuffer();
		}
		for (auto &it : monsters) window.draw(it);


		window.display();
		window.clear();

		if (SM.GetConnectionStatus() == sf::TcpSocket::Disconnected) {
			std::cout << "Disconnected" << std::endl;
			SM.Disconnect();
			//window.close();
		}
	}
	SM.Disconnect();
	getClientMessage.terminate();

	std::cout << "Connection stopped" << std::endl;
	//system("pause");
	return 0;
}