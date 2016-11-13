#pragma once
#include <vector>
#include "Joueur.h"
#include "Action.h"

class ActionMonopoly : public Action{
public:

	ActionMonopoly();
	void Play(Joueur& j);
	std::vector<Action*> getNextAction();
private:
	std::vector<Action*> nextActions;
};