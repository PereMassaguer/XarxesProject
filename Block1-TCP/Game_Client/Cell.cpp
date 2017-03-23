#include "Cell.h"



Cell::Cell()
{
	_perimeterWidth = 1;
	elementID = ElementID::CLEAN;

}

Cell::Cell(Coordinate pos) : Cell()
{
	//Cell graphics
	_pos = std::make_pair(pos.first * CELL_SIZE_X, pos.second * CELL_SIZE_Y);


	sf::RectangleShape t(sf::Vector2f(CELL_SIZE_X, _perimeterWidth));
	t.setFillColor(sf::Color(C_WHITE));

	//Upper
	t.setPosition(_pos.first, _pos.second);
	_perimeter.push_back(t);

	//Lower
	t.setPosition(_pos.first, _pos.second + CELL_SIZE_Y - _perimeterWidth);
	_perimeter.push_back(t);

	//Left
	t.setSize(sf::Vector2f(_perimeterWidth, CELL_SIZE_Y));
	t.setPosition(_pos.first, _pos.second);
	_perimeter.push_back(t);

	//Right
	t.setPosition(_pos.first + CELL_SIZE_X - _perimeterWidth, _pos.second);
	_perimeter.push_back(t);
}


Cell::~Cell()
{
}

void Cell::Draw(sf::RenderWindow &window)
{
	for(auto &it : _perimeter) window.draw(it);
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
