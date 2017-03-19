#include "WorldMap.h"

WorldMap::WorldMap()
{
	for (int i = 0; i < MAP_ROWS; i++)
		for (int j = 0; j < MAP_COLUMNS; j++) _cell[j][i] = Cell(std::make_pair(j, i));
 
	for (size_t i = 0; i < STARTING_UNITS; i++)
		_enemyUnits.push_back(Unit(Coordinate(-5, 5), ElementID::ENEMY_UNIT));

	for (auto &it : _enemyUnits) it.SetTexture(ElementID::ENEMY_UNIT);

	for (size_t i = 0; i < STARTING_BASES; i++)
		_enemyBases.push_back(Unit(Coordinate(-5, 5), ElementID::ENEMY_BASE));

	for (auto &it : _enemyBases) it.SetTexture(ElementID::ENEMY_BASE);

	_selectedUnit = nullptr; 
	movementsLeft = STARTING_UNITS;
}


WorldMap::~WorldMap()
{
}

int WorldMap::GetPlayerUnits()
{
	return _playerUnits.size();
}

int WorldMap::GetPlayerBases()
{
	return _playerBases.size();
}

void WorldMap::SetEnemyUnits(std::string t)
{
	t = t.substr(t.find('_') + 1, t.size());
	int units = stoi(t.substr(0, t.find('_')));
	t = t.substr(t.find('_'), t.size());

	Coordinate tCoord;
	for (auto &it : _enemyUnits)
	{
		t = t.substr(t.find('_') + 1, t.size());
		tCoord.first = MAP_COLUMNS - stoi(t.substr(0, t.find('_'))) - 1;
		t = t.substr(t.find('_') + 1, t.size());
		tCoord.second = stoi(t.substr(0, t.find('_')));
		std::cout << tCoord.first << ", " << tCoord.second << std::endl;
		it.SetPosition(tCoord);
	}
}

void WorldMap::SetEnemyBases(std::string t)
{
	t = t.substr(t.find('_') + 1, t.size());
	int units = stoi(t.substr(0, t.find('_')));
	t = t.substr(t.find('_'), t.size());

	Coordinate tCoord;
	for (auto &it : _enemyBases)
	{
		t = t.substr(t.find('_') + 1, t.size());
		tCoord.first = MAP_COLUMNS - stoi(t.substr(0, t.find('_'))) - 1;
		t = t.substr(t.find('_') + 1, t.size());
		tCoord.second = stoi(t.substr(0, t.find('_')));
		std::cout << tCoord.first << ", " << tCoord.second << std::endl;
		it.SetPosition(tCoord);
	}

}

void WorldMap::SetEnemyData(std::string t) {

	t = t.substr(t.find('_') + 1, t.size());
	int units = stoi(t.substr(0, t.find('_')));
	t = t.substr(t.find('_'), t.size());

	Coordinate tCoord;
	for (auto &it : _enemyUnits) {
		t = t.substr(t.find('_') + 1, t.size());
		tCoord.first = MAP_COLUMNS - stoi(t.substr(0, t.find('_'))) - 1;
		t = t.substr(t.find('_') + 1, t.size());
		tCoord.second = stoi(t.substr(0, t.find('_')));
		it.SetPosition(tCoord);

		t = t.substr(t.find('_') + 1, t.size());
		it.SetHp(stoi(t.substr(0, t.find('_'))));
		std::cout << tCoord.first << ", " << tCoord.second << " hp:" << stoi(t.substr(0, t.find('_'))) << std::endl;
	}
	
	t = t.substr(t.find('_') + 1, t.size());
	units = stoi(t.substr(0, t.find('_')));
	t = t.substr(t.find('_'), t.size());
	for (auto &it : _enemyBases) {
		t = t.substr(t.find('_') + 1, t.size());
		tCoord.first = MAP_COLUMNS - stoi(t.substr(0, t.find('_'))) - 1;
		t = t.substr(t.find('_') + 1, t.size());
		tCoord.second = stoi(t.substr(0, t.find('_')));
		std::cout << tCoord.first << ", " << tCoord.second << std::endl;
		it.SetPosition(tCoord);

		t = t.substr(t.find('_') + 1, t.size());
		it.SetHp(stoi(t.substr(0, t.find('_'))));
		std::cout << tCoord.first << ", " << tCoord.second << " hp:" << stoi(t.substr(0, t.find('_'))) << std::endl;
	}
}

std::vector<Coordinate> WorldMap::GetPlayerUnitsCoords()
{
	std::vector<Coordinate> v;
	for (auto it : _playerUnits) v.push_back(it._pos);
	return v;
}

std::vector<Coordinate> WorldMap::GetPlayerBasesCoords()
{
	std::vector<Coordinate> v;
	for (auto it : _playerBases) v.push_back(it._pos);
	return v;
}

int WorldMap::GetMovementsLeft()
{
	return movementsLeft;
}

void WorldMap::SetMovementsLeft(int i)
{
	movementsLeft = i;
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
				for (auto it : _playerBases) if (it._pos == coord) cellIsFree = false;
				for (auto it : _playerUnits) if (it._pos == coord) cellIsFree = false;
				for (auto it : _enemyBases) if (it._pos == coord) cellIsFree = false;
				for (auto it : _enemyUnits) if (it._pos == coord) cellIsFree = false;
				
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
	Coordinate coord = GetMousedCell(mouse);
	
	if (coord.first < 0 || coord.first >= MAP_COLUMNS || coord.second < 0 || coord.second >= MAP_ROWS) return;
	bool clear = true;
	switch (gameState)
	{
	case USER_CONNECTION:
		break;
	case NAME_INPUT:
		break;
	case TROOP_DEPLOY:
		if (mouse.button == sf::Mouse::Left) {
			if (coord.first < 5) {
				bool cellIsFree = true;
				for (auto it : _playerUnits)
					if (it._pos == coord) cellIsFree = false;

				if (_playerUnits.size() < STARTING_UNITS && cellIsFree) {
					_playerUnits.push_back(Unit(coord, ElementID::PLAYER_UNIT));
					for (auto &it : _playerUnits) it.SetTexture(ElementID::PLAYER_UNIT);
				}
			}
		}
		break;
	case BASE_DEPLOY:
		if (mouse.button == sf::Mouse::Left) {
			if (coord.first < 5) {
				bool cellIsFree = true;
				for (auto it : _playerBases) if (it._pos == coord) cellIsFree = false;
				for (auto it : _playerUnits) if (it._pos == coord) cellIsFree = false;

				if (_playerBases.size() < 2 && cellIsFree) {
					_playerBases.push_back(Unit(coord, ElementID::PLAYER_BASE));
					for (auto &it : _playerBases) it.SetTexture(ElementID::PLAYER_BASE);
				}
			}
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
				bool cellIsFree = true;
				for (auto it : _playerBases) if (it._pos == coord) cellIsFree = false;
				for (auto it : _playerUnits) if (it._pos == coord) cellIsFree = false;
				for (auto it : _enemyBases) if (it._pos == coord) cellIsFree = false;
				for (auto it : _enemyUnits) if (it._pos == coord) cellIsFree = false;

				if (cellIsFree) {
					for (auto it : _colouratedCells) {
						if (it == coord) {
							movementsLeft--;
							EraseCellColours();
							_selectedUnit->SetPosition(coord);
							_selectedUnit = nullptr;
							break;
						}
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


bool WorldMap::CellIsEmpty(Coordinate c)
{
	for (auto it : _playerUnits)
		if (it._pos == c) return false;

	for (auto it : _playerUnits)
		if (it._pos == c) return false;
}

void WorldMap::Draw(sf::RenderWindow & window)
{
	for (auto it : _playerUnits) it.Draw(window);
	for (auto it : _playerBases) it.Draw(window);
	for (auto it : _enemyUnits) it.Draw(window);
	for (auto it : _enemyBases) it.Draw(window);

	for (int i = 0; i < MAP_ROWS; i++) {
		for (int j = 0; j < MAP_COLUMNS; j++)
			_cell[j][i].Draw(window);
	}
}