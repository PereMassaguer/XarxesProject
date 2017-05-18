#pragma once
#include <vector>
#include <iostream>
#include <SFML\Network.hpp>


#define MAP_COLUMNS 24
#define MAP_ROWS 24
#define CELL_SIZE_X 30
#define CELL_SIZE_Y 30

#define WINDOW_X 1080
#define WINDOW_Y 720

#define PORT 5000
#define BUFFER_SIZE 1000

#define STARTING_UNITS 3
#define STARTING_BASES 2
#define STARTING_HP 3

#define GAME_SPEED 200;
#define PING 5000

typedef std::pair<int, int> Coordinate;

typedef std::pair<sf::IpAddress, unsigned short> ConnectionData;

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




#define P_START Start_ //Send name
#define P_SETUP Units_ //Send Nunits_, Xcoord_, Ycoord_(*Nunits) 



struct Player {
	int id;
	std::string name;
	int lastConCheck;
	int lastMovement;
	std::vector<Coordinate> tail;
	int stackedTail;
};




static void Debug(std::string str) {
	std::cout << str << std::endl;
}





static std::string StringedCoord(Coordinate c) {
	return std::to_string(c.first) + "_" + std::to_string(c.second);
}

static std::string StringedIDCoord(int id, Coordinate c) {
	return std::to_string(id) + "_" + StringedCoord(c);
}

static std::string StringedIDCoord(int id, std::vector<Coordinate> c) {
	std::string str = std::to_string(id);

	for (auto it : c) str += "_" + StringedCoord(it);

	return str;
}



static void StringToCoord(Coordinate &c, std::string &str) {
	c.first = stoi(str.substr(0, str.find("_")));

	str = str.substr(str.find("_") + 1, str.size());
	c.second = stoi(str.substr(0, str.find("_")));
	str = str.substr(str.find("_") + 1, str.size());
}

static void StringToCoord(std::vector<Coordinate> &c, std::string &str) {
	Coordinate tCoord;
	c.clear();
	while (str.length() > 3) {
		tCoord.first = stoi(str.substr(0, str.find("_")));
		str = str.substr(str.find("_") + 1, str.size());
		tCoord.second = stoi(str.substr(0, str.find("_")));
		str = str.substr(str.find("_") + 1, str.size());
		c.push_back(tCoord);
	}
}


static void StringToIDCoord(int &id, std::vector<Coordinate> &c, std::string &str) {
	id = stoi(str.substr(0, str.find("_")));
	str = str.substr(str.find("_") + 1, str.size());

	StringToCoord(c, str);
}

static void StringToIDCoord(int &id, Coordinate &c, std::string &str) {
	id = stoi(str.substr(0, str.find("_")));
	str = str.substr(str.find("_") + 1, str.size());

	StringToCoord(c, str);
}