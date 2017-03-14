#include "GraphicsManager.h"


GraphicsManager::GraphicsManager()
{
	sf::Vector2i screenDimensions(800, 600);

	window.create(sf::VideoMode(screenDimensions.x, screenDimensions.y), "Chat");

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
}

GraphicsManager::~GraphicsManager()
{
}

bool GraphicsManager::WindowIsOpen()
{
	return window.isOpen();
}

sf::Event GraphicsManager::PollEvent()
{
	sf::Event ret;
	window.pollEvent(ret);
	return ret;
}