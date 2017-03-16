#pragma once
#include <math.h>
#include <SFML\Graphics.hpp>


#include "Cell.h"
#include "GUIButton.h"
#include "ID.h"
#include "SocketManager.h"
#include "Unit.h"

class WorldMap
{
	Cell _cell[MAP_COLUMNS][MAP_ROWS];
	std::vector<Coordinate> _colouratedCells;

	std::vector<Unit> _playerUnits;
	Unit *_selectedUnit;


public:
	WorldMap();
	~WorldMap();


	Coordinate GetMousedCell(sf::Event::MouseButtonEvent &me);

	void ActivateCell(sf::Event::MouseButtonEvent mouse, GameState &gameState);

	void ColourateCell(Coordinate cellCoord, ColorID colour);
	void ColourateArea(Coordinate centerCoord, int range);
	void EraseCellColours();

	int GetPlayerUnits();
	void Draw(sf::RenderWindow &window);
};