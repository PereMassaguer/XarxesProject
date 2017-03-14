#pragma once
#include <SFML\Graphics.hpp>

#include "ID.h"

class Unit
{
	Coordinate _pos;
	sf::Texture _texture;
	sf::Sprite _sprite;

	friend class WorldMap;
public:
	Unit();
	Unit(Coordinate c);
	~Unit();

	void Draw(sf::RenderWindow &window);
	void SetPosition(Coordinate c);
};

