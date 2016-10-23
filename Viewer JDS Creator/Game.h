#pragma once
#include "Joueur.h"
#include "Objet.h"
#include <vector>
#include <glm/glm.hpp>

class Game {
public:
	Game();
	void init(int nbJoueur);
	void initZones();
	std::vector<Zone> getZones();
	std::vector<Joueur> getJoueurs();
	std::vector<Objet> getObjets();

private:
	std::vector<Joueur> joueurs;
	std::vector<Objet> objets;
	std::vector<Zone> zones;
};