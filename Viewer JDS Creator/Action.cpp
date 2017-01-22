#include "Action.h"
#include "LoadFile.h"

Action::Action() {
}

Action::Action(int id, std::string texture){
	this->id = id;
	this->filename = "./Assets/Monopoly/Actions/" + texture;
}

std::vector<Action*> Action::getNextAction() {
	return this->nextActions;
}
void Action::addNextAction(Action* nextAction) {
	this->nextActions.push_back(nextAction);
}
char* Action::getTexture() const {
	return (char*)this->filename.c_str();
}
void Action::setTextureId(unsigned int textureId) {
	this->textureId = textureId;
}
int Action::getId() const {
	return this->id;
}

unsigned int Action::getIdTexture() const {
	return this->textureId;
}

Bouton*  Action::getBouton() const {
	return this->bouton;
}
void  Action::setBouton(Bouton* bouton) {
	this->bouton = bouton;
	this->bouton->setAction(this);
}
/*Todo Put this elsewhere*/
void  Action::Play(Joueur* j) {
	int id = this->getId();
	switch (id) {
	//Init
	case 0:
		Action::addRessource(*j, Ressource::getRessourceId("Argent"), 1500);
		break;
	//Lancer les dés
	case 1: {
		bool isDouble = false;
		Action::getDoubleDice(6, ((JoueurMonopoly*)j)->nextDeplacement, ((JoueurMonopoly*)j)->nextDeplacementIsDouble);
		break;
	}
	//Se déplacer
	case 2: {
		Case* pCurrentCase = ((JoueurMonopoly*)j)->getCase(); 
		Case* pNextCase = NULL;
		for (int i = 0; i <  ((JoueurMonopoly*)j)->nextDeplacement; i++) {
			Action::Move(0, pCurrentCase, pNextCase);
			pCurrentCase = pNextCase;
		}
		((JoueurMonopoly*)j)->setCase(pNextCase);
	}
	}
}

bool Action::addRessource(Joueur& j, int iRessource, int value) {
	j.getRessource(iRessource).Add(value);
	return true;
}

bool Action::getDoubleDice(int nbFace, int& value, bool& isDouble) {
	int nb1 = rand() % nbFace + 1;
	int nb2 = rand() % nbFace + 1;
	isDouble = nb1 == nb2;
	value = nb1 + nb2;
	return true;
}

bool Action::Move(int iNextPosition, Case* pCurrentCase, Case *& pNextCase) {
	pNextCase = pCurrentCase->getNextCase();
	return true;
}