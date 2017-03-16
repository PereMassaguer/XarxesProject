#include "GUIButton.h"



GUIButton::GUIButton()
{
	_size = Coordinate(300, 75);
	_perimeterWidth = 5;
	SetReady(false);
}

#include <iostream>
GUIButton::GUIButton(Coordinate pos) : GUIButton()
{
	_pos = pos;
	_pos.first -= _size.first / 2;


	_backGround.setPosition(sf::Vector2f(_pos.first, _pos.second));
	_backGround.setSize(sf::Vector2f(_size.first, _size.second));
	SetColor();


	sf::Font font;
	if (!font.loadFromFile("courbd.ttf"))
	_message = sf::Text("trydfgsdghgsfjhmf", font, 24);
	_message.setFillColor(sf::Color(0, 160, 0));
	_message.setStyle(sf::Text::Bold);
	_message.setPosition(sf::Vector2f(100, 625));
	
	sf::RectangleShape t(sf::Vector2f(_pos.first, _perimeterWidth));
	t.setFillColor(sf::Color(C_WHITE));

	//Upper
	t.setPosition(_pos.first, _pos.second);
	_perimeter.push_back(t);

	//Lower
	t.setPosition(_pos.first, _pos.second + _size.second - _perimeterWidth);
	_perimeter.push_back(t);

	//Left
	t.setSize(sf::Vector2f(_perimeterWidth, _size.second));
	t.setPosition(_pos.first, _pos.second);
	_perimeter.push_back(t);

	//Right
	t.setPosition(_pos.first + _size.first - _perimeterWidth, _pos.second);
	_perimeter.push_back(t);
}

void GUIButton::Draw(sf::RenderWindow &window)
{
	window.draw(_backGround);
	for (auto &it : _perimeter) window.draw(it);
	window.draw(_message);
}

bool GUIButton::Moused(sf::Event::MouseButtonEvent mouse)
{
	if (mouse.x > _pos.first && mouse.x < _pos.first + _size.first) {
		if (mouse.y > _pos.second && mouse.y < _pos.second + _size.second)
			return true;
	}
	return false;
}

void GUIButton::SetMessage(std::string string)
{
	_message.setString(string);
}


void GUIButton::SetReady(bool v)
{
	_ready =  v;
	SetColor();
}

void GUIButton::SetColor()
{
	if (_ready)
		_backGround.setFillColor(sf::Color(C_GREEN));
	else
		_backGround.setFillColor(sf::Color(C_RED));	
}

bool GUIButton::CheckActivated(sf::Event::MouseButtonEvent mouse)
{
	if (_ready) {
		if (Moused(mouse)) {
			SetReady(false);
			return true;
		}
	}
	return false;
}


GUIButton::~GUIButton()
{
}