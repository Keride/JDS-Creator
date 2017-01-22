#pragma once
#include "Zone.h"
#include "Ressource.h"

class Joueur {
public:
	Joueur();
	Ressource& getRessource(int iRessource);

private: 
	Ressource* ressources; 
};