#pragma once
#include "ID.h"
#include "SocketManager.h"

class Game
{
	GameState _gameState;

	std::vector<Client> _clients;

	int _playerCounter;
	bool _playsA;
	bool _done;

public:
	void AddClient(Client *client);
	void FinishGame();
	void Run();
	Game();
	~Game();
};