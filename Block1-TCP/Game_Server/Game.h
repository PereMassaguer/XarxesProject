#pragma once
#include "ID.h"
#include "SocketManager.h"

class Game
{
	GameState gameState;

	std::vector<Player> players;

	int playerCounter;
	bool playsA;
	bool done;

public:
	Game();
	~Game();


	void Run();
	void ResolveCommand(std::string str);
};