#pragma once
#include "Zone.h"
#include "Ressource.h"

class Joueur {
public:
	Joueur(Zone* pzoneDepart);
	glm::vec3 getTranslation() const;
	Ressource& getRessource(int iRessource);

private: 
	Zone* plocation;
	Ressource* ressources;
};