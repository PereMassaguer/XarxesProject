#include "WorldMap.h"

WorldMap::WorldMap()
{
	//centers grid to X axis
	_gridDisplacement = Coordinate((WINDOW_X - CELL_SIZE_X * MAP_COLUMNS) / 2, 0);
	
	_cell = new Cell*[MAP_COLUMNS];
	for (int i = 0; i < MAP_COLUMNS; i++) {
		_cell[i] = new Cell[MAP_ROWS];
		for (int j = 0; j < MAP_ROWS; j++)
			_cell[i][j].SetPosition(Coordinate(i * CELL_SIZE_X + _gridDisplacement.first, j * CELL_SIZE_Y + _gridDisplacement.second));
	}
}



WorldMap::~WorldMap()
{

}


void WorldMap::ColourateCell(Coordinate cellCoord, ColorID colour)
{
	_cell[cellCoord.first][cellCoord.second].Colourate(colour);

	if (colour != ColorID::WHITE) _colouratedCells.push_back(cellCoord);
}

void WorldMap::ColourateArea(Coordinate centerCoord, int range)
{
	Coordinate clampedX(centerCoord.first, centerCoord.first), clampedY(centerCoord.second, centerCoord.second);

	clampedX.first -= range;
	clampedX.second += range;
	clampedY.first -= range;
	clampedY.second += range;
	if (clampedX.first < 0) clampedX.first = 0;
	if (clampedX.second >= MAP_COLUMNS) clampedX.second = MAP_COLUMNS - 1;
	if (clampedY.first < 0) clampedY.first = 0;
	if (clampedY.second >= MAP_ROWS) clampedY.second = MAP_ROWS - 1;

	for (int i = clampedX.first; i <= clampedX.second; i++) {
		for (int j = clampedY.first; j <= clampedY.second; j++) {
			Coordinate aux(abs(i - centerCoord.first), abs(j - centerCoord.second));
			float dist = sqrt((float)aux.first * (float)aux.first + (float)aux.second * (float)aux.second);
			if (dist <= range && dist > 0.2f) {
				bool cellIsFree = true;
				Coordinate coord(i, j);
				/*for (auto it : _playerBases) if (it._pos == coord) cellIsFree = false;
				for (auto it : _playerUnits) if (it._pos == coord) cellIsFree = false;
				for (auto it : _enemyBases) if (it._pos == coord) cellIsFree = false;
				for (auto it : _enemyUnits) if (it._pos == coord) cellIsFree = false;*/
				
				ColourateCell(coord, cellIsFree ? ColorID::GREEN : ColorID::RED);
			}
		}
	}
}

Coordinate WorldMap::GetMousedCell(sf::Event::MouseButtonEvent & me)
{
	return Coordinate(me.x / CELL_SIZE_X, me.y / CELL_SIZE_Y);
}

void WorldMap::ActivateCell(sf::Event::MouseButtonEvent mouse, GameState &gameState)
{
}

void WorldMap::EraseCellColours()
{
	for(auto &it : _colouratedCells)
		 _cell[it.first][it.second].Colourate(ColorID::WHITE);
	
	_colouratedCells.clear();
}


bool WorldMap::CellIsEmpty(Coordinate c)
{
	return true;
}

void WorldMap::Draw(sf::RenderWindow & window)
{
	for (int i = 0; i < MAP_COLUMNS; i++) {
		for (int j = 0; j < MAP_ROWS; j++)
			_cell[i][j].Draw(window);
	}
}