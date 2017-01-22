#include "JoueurMonopoly.h"



JoueurMonopoly::JoueurMonopoly(Case* pCaseDepart):Joueur(){
	this->pCurrentCase = pCaseDepart;
	this->nextDeplacement = 0;
}

glm::vec3 JoueurMonopoly::getTranslation() const {
	return this->pCurrentCase->getZone()->getCenter();
}

Case* JoueurMonopoly::getCase() const {
	return this->pCurrentCase;
}

void JoueurMonopoly::setCase(Case* pNextCase){
	this->pCurrentCase = pNextCase;
}
