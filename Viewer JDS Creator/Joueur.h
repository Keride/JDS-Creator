#pragma once
#include "Zone.h"

class Joueur {
	Joueur(Zone* pzoneDepart) {
		this->plocation = pzoneDepart;
	};

private: 
	Zone* plocation;
};