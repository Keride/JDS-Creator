#pragma once
#include <vector>
#include "Joueur.h"

class Action {
public:
	Action();
	Action(int id);
	virtual void Play(Joueur& j) {};
	std::vector<Action*> getNextAction();
	bool static addRessource(Joueur j, int iRessource, int value);
protected:
	int id;
private:
	std::vector<Action*> nextActions;
};