#pragma once
#include <SFML\Graphics.hpp>
#include "ID.h"

class Unit
{
	Coordinate _pos;
	sf::Texture _texture;
	sf::Sprite _sprite;
	ElementID _id;

	int _hp;

	friend class WorldMap;
public:
	Unit();
	Unit(Coordinate c, ElementID id);
	~Unit();

	void Draw(sf::RenderWindow &window);
	void SetPosition(Coordinate c);
	void SetHp(int i);
	void SetTexture(ElementID id);
};

