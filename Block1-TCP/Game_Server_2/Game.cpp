#include "Game.h"

void Game::AddClient(Client * client)
{
	_clients.push_back(*client);
}

void Game::FinishGame()
{
	for (auto it : _clients) {
		it.playing = false;
	}
	_clients.clear();
}

Game::Game() {
	_gameState = GameState::USER_CONNECTION;
	_playerCounter = 0;
	_playsA = true;
	_done = false;
}

Game::~Game() {
	FinishGame();
}