#include "Unit.h"



Unit::Unit()
{
	_texture.loadFromFile("Rick.jpg");
	_sprite.setTexture(_texture);

	_pos.first = 5;
	_pos.second = 5;
	_sprite.setPosition(_pos.first * CELL_SIZE_X, _pos.second * CELL_SIZE_Y);
}

Unit::Unit(Coordinate c) : Unit()
{
	_pos = c;
}


Unit::~Unit()
{
}

void Unit::Draw(sf::RenderWindow & window)
{
	_sprite.setPosition(_pos.first * CELL_SIZE_X, _pos.second * CELL_SIZE_Y);
	window.draw(_sprite);
}

void Unit::SetPosition(Coordinate c)
{
	_pos = c;
}
