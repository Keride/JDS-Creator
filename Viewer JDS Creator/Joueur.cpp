#include "Joueur.h"

Joueur::Joueur(Zone* pzoneDepart) {

	this->plocation = pzoneDepart;
}

glm::vec3 Joueur::getTranslation() const {
	return this->plocation->getCenter();
}