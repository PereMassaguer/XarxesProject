#include "Game.h"

Game::Game() {
	_gameState = GameState::USER_CONNECTION;
	_playerCounter = 0;
	_playsA = true;
	_done = false;
}




Game::~Game() {

}
