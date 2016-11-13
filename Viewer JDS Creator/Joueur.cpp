#include "Joueur.h"

Joueur::Joueur(Zone* pzoneDepart) {

	this->plocation = pzoneDepart;
	this->ressources = new Ressource[Ressource::getnbRessource()];
}

glm::vec3 Joueur::getTranslation() const {
	return this->plocation->getCenter();
}

Ressource& Joueur::getRessource(int iRessource) {
	return this->ressources[iRessource];
}
