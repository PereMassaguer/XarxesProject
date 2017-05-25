#pragma once
#include "ID.h"
#include "Game.h"
#include "SocketManager.h"
class Lobby
{
	std::vector<Client> _clients;

	std::vector<Game> _games;


public:
	Lobby();
	~Lobby();
};

