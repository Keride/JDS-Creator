#include "ActionMonopoly.h"

ActionMonopoly::ActionMonopoly() {}

void ActionMonopoly::Play(Joueur& j) {
	if(this->id == 0)
		Action::addRessource(j, Ressource::getRessourceId("Argent"), 1500);
}