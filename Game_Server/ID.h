#pragma once
#include <vector>


#define MAP_COLUMNS 15
#define MAP_ROWS 10
#define CELL_SIZE_X 60
#define CELL_SIZE_Y 60


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