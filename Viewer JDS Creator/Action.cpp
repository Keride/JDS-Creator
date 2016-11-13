#include "Action.h"
Action::Action() {}

Action::Action(int id){
	this->id == id;
}
std::vector<Action*> Action::getNextAction() {
	return nextActions;
}

bool Action::addRessource(Joueur j, int iRessource, int value) {
	j.getRessource(iRessource).Add(value);
	return true;
}