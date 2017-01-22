#include "Joueur.h"

Joueur::Joueur() {
	this->ressources = new Ressource[Ressource::getnbRessource()];
}

Ressource& Joueur::getRessource(int iRessource) {
	return this->ressources[iRessource];
}
