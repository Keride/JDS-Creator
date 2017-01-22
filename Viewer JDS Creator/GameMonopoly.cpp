#include "GameMonopoly.h"
#include "Bouton.h"

GameMonopoly::GameMonopoly() {};

void GameMonopoly::initZones() {
	float x = 1.0f;
	float z = -1.0f;
	float sizeX = 0.25f;
	float sizeZ = 0.25f;
	//-------
	cases.push_back(new Case(new Zone(glm::vec3(x, 0.0f, z), glm::vec3(x - sizeX, 0.0f, z), glm::vec3(x - sizeX, 0.0f, z + sizeZ), glm::vec3(x, 0.0f, z + sizeZ))));
	x -= sizeX;
	sizeX = 0.1666f;
	for (int i = 0; i < 9; i++) {
		cases.push_back(new Case(new Zone(glm::vec3(x, 0.0f, z), glm::vec3(x - sizeX, 0.0f, z), glm::vec3(x - sizeX, 0.0f, z + sizeZ), glm::vec3(x, 0.0f, z + sizeZ))));
		x -= sizeX;
	}
	sizeX = 0.25f;
	sizeZ = 0.25f;
	cases.push_back(new Case(new Zone(glm::vec3(x, 0.0f, z), glm::vec3(x - sizeX, 0.0f, z), glm::vec3(x - sizeX, 0.0f, z + sizeZ), glm::vec3(x, 0.0f, z + sizeZ))));
	//-----
	z += sizeZ;
	sizeZ = 0.1666f;
	for (int i = 0; i < 9; i++) {
		cases.push_back(new Case(new Zone(glm::vec3(x, 0.0f, z), glm::vec3(x - sizeX, 0.0f, z), glm::vec3(x - sizeX, 0.0f, z + sizeZ), glm::vec3(x, 0.0f, z + sizeZ))));
		z += sizeZ;
	}
	sizeX = 0.25f;
	sizeZ = 0.25f;
	cases.push_back(new Case(new Zone(glm::vec3(x, 0.0f, z), glm::vec3(x - sizeX, 0.0f, z), glm::vec3(x - sizeX, 0.0f, z + sizeZ), glm::vec3(x, 0.0f, z + sizeZ))));
	//-----
	sizeX = 0.1666f;
	x += sizeX;
	for (int i = 0; i < 9; i++) {
		cases.push_back(new Case(new Zone(glm::vec3(x, 0.0f, z), glm::vec3(x - sizeX, 0.0f, z), glm::vec3(x - sizeX, 0.0f, z + sizeZ), glm::vec3(x, 0.0f, z + sizeZ))));
		x += sizeX;
	}
	x += 0.25f - sizeX;
	sizeX = 0.25f;
	sizeZ = 0.25f;
	cases.push_back(new Case(new Zone(glm::vec3(x, 0.0f, z), glm::vec3(x - sizeX, 0.0f, z), glm::vec3(x - sizeX, 0.0f, z + sizeZ), glm::vec3(x, 0.0f, z + sizeZ))));
	//-----
	sizeZ = 0.1666f;
	z -= sizeZ;
	for (int i = 0; i < 9; i++) {
		cases.push_back(new Case(new Zone(glm::vec3(x, 0.0f, z), glm::vec3(x - sizeX, 0.0f, z), glm::vec3(x - sizeX, 0.0f, z + sizeZ), glm::vec3(x, 0.0f, z + sizeZ))));
		z -= sizeZ;
	}

	for (std::size_t i = 0; i < cases.size()-1; i++) {
		cases[i]->SetNextCase(cases[i+1]);
	}
	cases[cases.size() - 1]->SetNextCase(cases[0]);
}

void GameMonopoly::initUIZones() {
	float x = 0.3f;
	float y = 0.9f;
	float sizeX = 0.15f;
	float sizeY = 0.1f;
	for (int i = 0; i < this->nextPossibleActions.size(); i++) {
		this->nextPossibleActions[i]->setBouton(new Bouton(glm::vec3(x, y, 0), glm::vec3(x + sizeX, y, 0), glm::vec3(x + sizeX, y + sizeY, 0), glm::vec3(x, y + sizeY, 0)));
		x += 0.2;
	}
	/*x = 0.7f;
	y = 0.9f;
	this->actions[1]->setBouton(new Bouton(glm::vec3(x, y, 0), glm::vec3(x + sizeX, y, 0), glm::vec3(x + sizeX, y + sizeY, 0), glm::vec3(x, y + sizeY, 0)));*/
}

std::vector<Case*> GameMonopoly::getCases() const {
	return this->cases;
}
void GameMonopoly::initActions() {
	this->actions.push_back(new Action(1, "des.png"));
	this->initGameAction.addNextAction(this->actions[0]);
	this->actions.push_back(new Action(2, "deplacement.png"));
	this->actions[0]->addNextAction(this->actions[1]);
	this->actions.push_back(new Action(3, "acheterQuartier.png"));
	this->actions[1]->addNextAction(this->actions[2]);
	this->actions.push_back(new Action(4, "payer.png"));
	this->actions[1]->addNextAction(this->actions[3]);
	this->actions.push_back(new Action(5, "card-draw.png"));
}

void GameMonopoly::init(int nbJoueur) {
	this->nbJoueur = nbJoueur;
	this->idJoueurActuel = 0;
	this->initZones();

	this->initActions();
	this->initUIZones();

	Ressource::addRessource("Argent");

	for (int i = 0; i < nbJoueur; i++) {
		this->joueurs.push_back(new JoueurMonopoly(cases[0]));
	}

}

void GameMonopoly::initGame(){
	Game::initGame();
}