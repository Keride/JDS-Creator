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
	std::vector<Case*> getCases() const ;
private:
	std::vector<Case*> cases;
};