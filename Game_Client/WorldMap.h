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
	std::vector<Unit> _playerBases;
	Unit *_selectedUnit;
	
	std::vector<Unit> _enemyUnits;
	std::vector<Unit> _enemyBases;

	int movementsLeft;
public:
	WorldMap();
	~WorldMap();


	Coordinate GetMousedCell(sf::Event::MouseButtonEvent &me);

	void ActivateCell(sf::Event::MouseButtonEvent mouse, GameState &gameState);

	void ColourateCell(Coordinate cellCoord, ColorID colour);
	void ColourateArea(Coordinate centerCoord, int range);
	void EraseCellColours();

	bool CellIsEmpty(Coordinate c);

	int GetPlayerUnits();
	int GetPlayerBases();
	void SetEnemyUnits(std::string t);
	void SetEnemyBases(std::string t);
	void SetEnemyData(std::string t);
	std::vector<Coordinate> GetPlayerUnitsCoords();
	std::vector<Coordinate> GetPlayerBasesCoords();

	int GetMovementsLeft();
	void SetMovementsLeft(int i);

	void Draw(sf::RenderWindow &window);
};