#include "GUIButton.h"



GUIButton::GUIButton()
{
	_perimeterWidth = 5;
	SetReady(false);

	SetColor();
	
	if (!font.loadFromFile("courbd.ttf")) std::cout << "Can't load the font file" << std::endl;
	_message = sf::Text("", font, 24);
	_message.setFillColor(sf::Color(255, 255, 255));
	_message.setStyle(sf::Text::Bold);
}

#include <iostream>
GUIButton::GUIButton(Coordinate pos, Coordinate size) : GUIButton()
{
	_pos = pos;
	_size = size;


	_backGround.setPosition(sf::Vector2f(_pos.first, _pos.second));
	_backGround.setSize(sf::Vector2f(_size.first, _size.second));
	_message.setPosition(sf::Vector2f(_pos.first + _size.first / 2, _pos.second + _size.second/2));
	
	sf::RectangleShape t(sf::Vector2f(_size.first, _perimeterWidth));
	t.setFillColor(sf::Color(C_WHITE));

	//Upper
	t.setPosition(_pos.first, _pos.second);
	_perimeter.push_back(t);

	//Lower
	t.setOrigin(0, -_size.second + _perimeterWidth);
	_perimeter.push_back(t);

	//Left
	t.setSize(sf::Vector2f(_perimeterWidth, _size.second));
	t.setOrigin(0, 0);
	_perimeter.push_back(t);

	//Right
	t.setOrigin(-_size.first + _perimeterWidth, 0);
	_perimeter.push_back(t);
}

GUIButton::GUIButton(sf::String str, Coordinate pos, Coordinate size) : GUIButton(pos, size) {
	_message.setString(str);
	sf::FloatRect textRect = _message.getLocalBounds();
	_message.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
}

void GUIButton::Draw(sf::RenderWindow &window)
{
	window.draw(_backGround);
	for (auto it : _perimeter) window.draw(it);

	if (_message.getString().toAnsiString() != "") window.draw(_message);
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