#include "Game.h"

Game::Game() {

}

void Game::initGame() {
}

std::vector<Zone*> Game::getZones() {
	return this->zones;
}
std::vector<Zone*> Game::getUIZones() {
	return this->zonesUI;
}
std::vector<Joueur> Game::getJoueurs() {
	return this->joueurs;
}
std::vector<Objet> Game::getObjets() {
	return this->objets;
}