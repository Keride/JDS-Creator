#pragma once
#include <vector>
#include "Joueur.h"
#include "JoueurMonopoly.h"
#include "Bouton.h"

class Action {
public:
	Action();
	Action(int id, std::string texture);
	std::vector<Action*> getNextAction();
	bool static addRessource(Joueur& j, int iRessource, int value);
	bool static getDoubleDice(int nbFace, int& value, bool & isDouble);
	bool static Move(int iNextPosition, Case* pCurrentCase, Case*& pNextCase);
	int getId() const;
	char* getTexture() const;
	unsigned int getIdTexture() const;
	void setTextureId(unsigned int textureId);
	void addNextAction(Action* nextAction);

	Bouton* getBouton() const;
	void setBouton(Bouton* bouton);
	void Play(Joueur* j);

protected:
	int id;
private:
	std::string filename;
	unsigned int textureId;
	std::vector<Action*> nextActions;
	Bouton* bouton;
};