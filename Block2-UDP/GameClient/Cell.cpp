#include "Cell.h"



Cell::Cell()
{
	_perimeterWidth = 1;
	elementID = ElementID::CLEAN;

	sf::RectangleShape t(sf::Vector2f(CELL_SIZE_X, 2));
	t.setFillColor(sf::Color(C_WHITE));

	_perimeter.push_back(t);
	_perimeter.push_back(t);
	t.setSize(sf::Vector2f(_perimeterWidth, CELL_SIZE_Y));
	_perimeter.push_back(t);
	_perimeter.push_back(t);
}


Cell::Cell(Coordinate pos) : Cell() {
	SetPosition(pos);
}


Cell::~Cell()
{
}

void Cell::SetPosition(Coordinate pos)
{
	//Cell graphics
	_pos = pos;

	//Upper
	_perimeter[0].setPosition(_pos.first, _pos.second);

	//Lower
	_perimeter[1].setPosition(_pos.first, _pos.second + CELL_SIZE_Y - _perimeterWidth / 2);

	//Left
	_perimeter[2].setPosition(_pos.first, _pos.second);

	//Right
	_perimeter[3].setPosition(_pos.first + CELL_SIZE_X - _perimeterWidth / 2, _pos.second);
}

void Cell::Draw(sf::RenderWindow &window)
{
	for(auto &it : _perimeter)
		window.draw(it);
}

void Cell::Colourate(ColorID colour)
{
	switch (colour)
	{
	case WHITE:
		for (auto &it : _perimeter) 
			it.setFillColor(sf::Color(C_WHITE));
		break;
	case RED:
		for (auto &it : _perimeter)
			it.setFillColor(sf::Color(C_RED));
		break;
	case GREEN:
		for (auto &it : _perimeter)
			it.setFillColor(sf::Color(C_GREEN));
		break;
	case BLUE:
		for (auto &it : _perimeter)
			it.setFillColor(sf::Color(C_BLUE));
		break;
	default:
		break;
	}		
}
