#pragma once
#include "Joueur.h"
#include "Action.h"
#include "Objet.h"
#include "Ressource.h"
#include <vector>
#include <map>
#include <glm/glm.hpp>

class Game {
public:
	Game();
	virtual void init(int nbJoueur)=0;
	virtual void initGame();
	void updateActions(Action* action);
	virtual void initUIZones() = 0;
	virtual void initZones() =0;
	virtual void initActions()=0;
	Joueur* getJoueurActuel();
	std::vector<Zone*> getZones();
	std::vector<Joueur*> getJoueurs();
	std::vector<Objet> getObjets();
	std::vector<Action*> getActions();
	std::vector<Action*> getNextPossibleActions();

protected:
	int nbJoueur;
	std::vector<Joueur*> joueurs;
	std::vector<Objet> objets;
	std::vector<Zone*> zones;
	Action initGameAction;
	Action endGameAction;
	std::vector<Action*> actions;
	std::vector<Action*> nextPossibleActions;
	int idJoueurActuel;
};