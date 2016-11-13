#pragma once
#include <vector>
#include "Game.h"

class GameMonopoly : public Game{
public:
	GameMonopoly();
	void init(int nbJoueur);
	void initGame();
	void initZones();
	void initUIZones();
	void initActions();
private:

	ActionMonopoly initGameAction;
	std::vector<ActionMonopoly> actions;
};