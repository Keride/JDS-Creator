#include "Game.h"

Game::Game() {

}

void Game::initGame() {
	for (int i = 0; i < nbJoueur; i++) {
		initGameAction.Play(this->joueurs[i]);
	}
	this->updateActions(&initGameAction);
}

void Game::updateActions(Action* action) {
	this->nextPossibleActions.clear();
	std::vector<Action*> nextActions = action->getNextAction();
	for (int i = 0; i < nextActions.size(); i++) {
		this->nextPossibleActions.push_back(nextActions[i]);
	}
	this->initUIZones();
}
std::vector<Zone*> Game::getZones() {
	return this->zones;
}

Joueur* Game::getJoueurActuel() {
	return this->joueurs[idJoueurActuel];
}

std::vector<Action*> Game::getNextPossibleActions() {
	return this->nextPossibleActions;
}
std::vector<Joueur*> Game::getJoueurs() {
	return this->joueurs;
}
std::vector<Objet> Game::getObjets() {
	return this->objets;
}

std::vector<Action*> Game::getActions() {
	return this->actions;
}