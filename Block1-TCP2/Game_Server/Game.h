#pragma once
#include "ID.h"
#include "SocketManager.h"

class Game
{
	std::vector<Player*> _players;

	GameState _gameState;

	bool _playsA;
	bool _done;

public:
	Game();
	~Game();

	int players;

	//void Run();
	void AddPlayer(Player * player);
	void RunCommand(int playerId, std::string com);
};