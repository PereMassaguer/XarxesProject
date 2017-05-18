#pragma once
#include <SFML\Graphics.hpp>
#include <vector>

#include "ID.h"

class Cell
{
	Coordinate _pos;
	int _perimeterWidth;
	std::vector<sf::RectangleShape> _perimeter;

	ElementID elementID;

public:
	Cell();
	Cell(Coordinate pos);
	~Cell();

	void Draw(sf::RenderWindow &window);
	void Colourate(ColorID color);
};