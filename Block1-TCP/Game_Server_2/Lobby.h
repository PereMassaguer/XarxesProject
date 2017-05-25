#pragma once
#include "ID.h"
#include "Game.h"
#include "SocketManager.h"


#define LM Lobby::Instance()
class Lobby
{
	std::vector<Client*> _clients;

	std::vector<Game> _games;


public:
	Lobby();
	~Lobby();

	inline static Lobby &Instance() {
		static Lobby lob;
		return lob;
	}
};

