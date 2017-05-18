#include "Unit.h"

#include <iostream>

Unit::Unit()
{
	_pos.first = 5;
	_pos.second = 5;
	_sprite.setPosition(_pos.first * CELL_SIZE_X, _pos.second * CELL_SIZE_Y);

	_hp = STARTING_HP;
}

Unit::Unit(Coordinate c, ElementID id) : Unit()
{
	_id = id;
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

void Unit::SetHp(int i)
{
	_hp = i;
}


void Unit::SetTexture(ElementID id) {
	switch (_id)
	{
	case PLAYER_UNIT:
		if (!_texture.loadFromFile("AUnit.jpg"))
			std::cout << "fail on image load" << std::endl;
		break;
	case PLAYER_BASE:
		if (!_texture.loadFromFile("ABase.jpg"))
			std::cout << "fail on image load" << std::endl;
		break;
	case ENEMY_UNIT:
		if (!_texture.loadFromFile("BUnit.jpg"))
			std::cout << "fail on image load" << std::endl;
		break;
	case ENEMY_BASE:
		if (!_texture.loadFromFile("BBase.jpg"))
			std::cout << "fail on image load" << std::endl;
		break;
	default:
		break;
	}
	_sprite.setTexture(_texture);
}