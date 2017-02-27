#pragma once
#include <iostream>
#include <SFML\Graphics.hpp>
#include <SFML\Network.hpp>



#define GM GraphicsManager::Instance()
class GraphicsManager
{
	sf::RenderWindow window;
	sf::String mensaje;

public:
	GraphicsManager();
	~GraphicsManager();

	inline static GraphicsManager &Instance() {
		static GraphicsManager graphicsManager;
		return graphicsManager;
	}

	bool WindowIsOpen();
	sf::Event PollEvent();
};