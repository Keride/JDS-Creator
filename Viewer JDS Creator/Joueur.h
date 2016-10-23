#pragma once
#include "Zone.h"

class Joueur {
public:
	Joueur(Zone* pzoneDepart);
	glm::vec3 getTranslation() const;

private: 
	Zone* plocation;
};