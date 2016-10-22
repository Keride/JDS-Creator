#pragma once
#include "Joueur.h"
#include "Objet.h"
#include <vector>

class Game {
public:
	Game();
	void init();

private:
	std::vector<Joueur> joueurs;
	std::vector<Objet> objets;
};