#pragma once
#include <vector>
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
	LOBBY, 
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



static std::string CoordToString(Coordinate c) {
	return std::to_string(c.first) + "_" + std::to_string(c.second);
}



static void Debug(std::string str) {
	std::cout << str << std::endl;
}