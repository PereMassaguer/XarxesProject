#pragma once
#include <math.h>
#include <SFML\Graphics.hpp>


#include "Cell.h"
#include "GUIButton.h"
#include "ID.h"

class WorldMap
{
	Cell** _cell;
	Coordinate _gridDisplacement;

	std::vector<Coordinate> _colouratedCells;


public:
	WorldMap::WorldMap();
	~WorldMap();


	Coordinate GetMousedCell(sf::Event::MouseButtonEvent &me);

	void ActivateCell(sf::Event::MouseButtonEvent mouse, GameState &gameState);

	void ColourateCell(Coordinate cellCoord, ColorID colour);
	void ColourateArea(Coordinate centerCoord, int range);
	void EraseCellColours();

	bool CellIsEmpty(Coordinate c);


	void Draw(sf::RenderWindow &window);
};