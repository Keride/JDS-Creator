#pragma once
#include "Joueur.h"
#include "Objet.h"
#include "Ressource.h"
#include "ActionMonopoly.h"
#include <vector>
#include <map>
#include <glm/glm.hpp>

class Game {
public:
	Game();
	virtual void init(int nbJoueur)=0;
	virtual void initGame();
	virtual void initUIZones() = 0;
	virtual void initZones() =0;
	virtual void initActions()=0;
	std::vector<Zone*> getZones();
	std::vector<Zone*> getUIZones();
	std::vector<Joueur> getJoueurs();
	std::vector<Objet> getObjets();

protected:
	int nbJoueur;
	std::vector<Joueur> joueurs;
	std::vector<Objet> objets;
	std::vector<Zone*> zones;
	std::vector<Zone*> zonesUI;
	int idJoueurActuel;
};