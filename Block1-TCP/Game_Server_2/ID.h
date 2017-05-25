#pragma once
#include <vector>
#include <string>
#include <SFML\Graphics.hpp>
#include <SFML\Network.hpp>
#include <iostream>

#define MAP_COLUMNS 15
#define MAP_ROWS 10
#define CELL_SIZE_X 60
#define CELL_SIZE_Y 60

#define STARTING_UNITS 3
#define STARTING_BASES 2
#define STARTING_HP 3


typedef std::pair<int, int> Coordinate;

enum ElementID {
	CLEAN,
	PLAYER_UNIT,
	PLAYER_BASE,
	ENEMY_UNIT,
	ENEMY_BASE
};


enum GameState {
	USER_CONNECTION,
	NAME_INPUT,
	TROOP_DEPLOY,
	BASE_DEPLOY,
	START_GAME,
	GAME_LOOP
};


enum ColorID {
	WHITE,
	RED,
	GREEN,
	BLUE
};
#define C_WHITE 200, 200, 200, 255
#define C_RED 255, 0, 0, 255
#define C_GREEN 0, 255, 0, 255
#define C_BLUE 0, 0, 255, 255
#define C_BLACK 0, 0, 0, 255



struct Player {
	int id;
	sf::Text name;
	std::vector<std::pair<Coordinate, int>> units;
	std::vector<std::pair<Coordinate, int>> base;
};

struct Client {
	bool playing;
	sf::IpAddress ip;
	sf::TcpSocket* socket;
	char buffer[2000];
	Player player;
};

static std::string CoordToString(Coordinate c) {
	return std::to_string(c.first) + "_" + std::to_string(c.second);
}

static void StringToCoord(Coordinate &c, std::string &str) {
	c.first = stoi(str.substr(0, str.find('_')));
	str = str.substr(str.find('_') + 1, str.size());
	c.second = stoi(str.substr(0, str.find('_')));
}

static void Debug(std::string str) {
	std::cout << str << std::endl;
}