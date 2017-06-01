#pragma once
#include <SFML\Graphics.hpp>
#include <vector>


#include "ID.h"

class GUIButton
{
	sf::Text _message;
	sf::RectangleShape _backGround;
	sf::Color _color;
	sf::Font font;
		
	Coordinate _pos;
	Coordinate _size;
	int _perimeterWidth;
	std::vector<sf::RectangleShape> _perimeter;

	bool _ready;

public:
	GUIButton();
	GUIButton(Coordinate pos, Coordinate size);
	GUIButton(sf::String str, Coordinate pos, Coordinate size);
	~GUIButton();

	void Draw(sf::RenderWindow & window);
	bool Moused(sf::Event::MouseButtonEvent mouse);
	void SetMessage(std::string string);
	void SetReady(bool v);
	void SetColor();
	bool CheckActivated(sf::Event::MouseButtonEvent mouse);
};

