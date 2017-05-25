#pragma once
#include "ID.h"
#include "SocketManager.h"

class Game
{
	GameState _gameState;

	std::vector<Player> _players;

	int _playerCounter;
	bool _playsA;
	bool _done;

public:
	Game();
	~Game();
};