#include "WorldMap.h"
#include <iostream>

WorldMap::WorldMap()
{
	for (int i = 0; i < MAP_ROWS; i++)
		for (int j = 0; j < MAP_COLUMNS; j++) _cell[j][i] = Cell(std::make_pair(j, i));


	_selectedUnit = nullptr; 
}


WorldMap::~WorldMap()
{
}

void WorldMap::Draw(sf::RenderWindow & window)
{
	for (auto it : _playerUnits) it.Draw(window);

	for (int i = 0; i < MAP_ROWS; i++) {
		for (int j = 0; j < MAP_COLUMNS; j++) {
			_cell[j][i].Draw(window);
		}
	}
}

void WorldMap::ColourateCell(Coordinate cellCoord, ColorID colour)
{
	_cell[cellCoord.first][cellCoord.second].Colourate(colour);

	if (colour != ColorID::WHITE) 
		_colouratedCells.push_back(cellCoord);
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
				ColourateCell(Coordinate(i, j), ColorID::GREEN);
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
	Coordinate coord = GetMousedCell(mouse);
	bool clear = true;
	switch (gameState)
	{
	case USER_CONNECTION:
		break;
	case NAME_INPUT:
		break;
	case TROOP_DEPLOY:
		if (mouse.button == sf::Mouse::Left) {
			if (_playerUnits.size() < 3) {
				_playerUnits.push_back(Unit(coord));
			}
			if (_playerUnits.size() == 3)
				gameState = GameState::GAME_LOOP;
		}
		break;
	case GAME_LOOP:
		if (mouse.button == sf::Mouse::Left) {
			if (_selectedUnit == nullptr) {
				EraseCellColours();
				for (int i = 0; i < _playerUnits.size(); i++) {
					if (_playerUnits[i]._pos == coord) {
						ColourateArea(coord, 2);
						clear = false;
						_selectedUnit = new Unit();
						_selectedUnit = &_playerUnits[i];
						break;
					}
				}
				if (clear) ColourateCell(coord, ColorID::GREEN);
			}
			else {
				for (auto it : _colouratedCells) {
					if (it.first == coord.first && it.second == coord.second) {
						EraseCellColours();
						_selectedUnit->SetPosition(coord);
						_selectedUnit = nullptr;
					}
				}
			}
		}
		break;
	default:
		break;
	}	
}

void WorldMap::EraseCellColours()
{
	for(auto &it : _colouratedCells)
		 _cell[it.first][it.second].Colourate(ColorID::WHITE);
	
	_colouratedCells.clear();
}