#include "GameMonopoly.h"
#include "Bouton.h"

GameMonopoly::GameMonopoly() {};

void GameMonopoly::initZones() {
	float x = 1.0f;
	float z = 1.0f;
	float sizeX = 0.25f;
	float sizeZ = 0.25f;
	//-------
	zones.push_back(new Zone(glm::vec3(x, 0.0f, z), glm::vec3(x - sizeX, 0.0f, z), glm::vec3(x - sizeX, 0.0f, z - sizeZ), glm::vec3(x, 0.0f, z - sizeZ)));
	x -= sizeX;
	sizeX = 0.1666;
	for (int i = 0; i < 9; i++) {
		zones.push_back(new Zone(glm::vec3(x, 0.0f, z), glm::vec3(x - sizeX, 0.0f, z), glm::vec3(x - sizeX, 0.0f, z - sizeZ), glm::vec3(x, 0.0f, z - sizeZ)));
		x -= sizeX;
	}
	sizeX = 0.25f;
	sizeZ = 0.25f;
	zones.push_back(new Zone(glm::vec3(x, 0.0f, z), glm::vec3(x - sizeX, 0.0f, z), glm::vec3(x - sizeX, 0.0f, z - sizeZ), glm::vec3(x, 0.0f, z - sizeZ)));
	//-----
	z -= sizeZ;
	sizeZ = 0.1666;
	for (int i = 0; i < 9; i++) {
		zones.push_back(new Zone(glm::vec3(x, 0.0f, z), glm::vec3(x - sizeX, 0.0f, z), glm::vec3(x - sizeX, 0.0f, z - sizeZ), glm::vec3(x, 0.0f, z - sizeZ)));
		z -= sizeZ;
	}
	sizeX = 0.25f;
	sizeZ = 0.25f;
	zones.push_back(new Zone(glm::vec3(x, 0.0f, z), glm::vec3(x - sizeX, 0.0f, z), glm::vec3(x - sizeX, 0.0f, z - sizeZ), glm::vec3(x, 0.0f, z - sizeZ)));
	//-----
	sizeX = 0.1666;
	x += sizeX;
	for (int i = 0; i < 9; i++) {
		zones.push_back(new Zone(glm::vec3(x, 0.0f, z), glm::vec3(x - sizeX, 0.0f, z), glm::vec3(x - sizeX, 0.0f, z - sizeZ), glm::vec3(x, 0.0f, z - sizeZ)));
		x += sizeX;
	}
	x += 0.25 - sizeX;
	sizeX = 0.25f;
	sizeZ = 0.25f;
	zones.push_back(new Zone(glm::vec3(x, 0.0f, z), glm::vec3(x - sizeX, 0.0f, z), glm::vec3(x - sizeX, 0.0f, z - sizeZ), glm::vec3(x, 0.0f, z - sizeZ)));
	//-----
	sizeZ = 0.1666;
	z += sizeZ;
	for (int i = 0; i < 9; i++) {
		zones.push_back(new Zone(glm::vec3(x, 0.0f, z), glm::vec3(x - sizeX, 0.0f, z), glm::vec3(x - sizeX, 0.0f, z - sizeZ), glm::vec3(x, 0.0f, z - sizeZ)));
		z += sizeZ;
	}
}

void GameMonopoly::initUIZones() {
	float x = 0.5f;
	float y = 0.9f;
	float sizeX = 0.15f;
	float sizeY = 0.1f;
	zonesUI.push_back(new Bouton(glm::vec3(x, y, 0), glm::vec3(x + sizeX, y, 0), glm::vec3(x + sizeX, y + sizeY, 0), glm::vec3(x, y + sizeY, 0)));
	x = 0.7f;
	y = 0.9f;
	zonesUI.push_back(new Bouton(glm::vec3(x, y, 0), glm::vec3(x + sizeX, y, 0), glm::vec3(x + sizeX, y + sizeY, 0), glm::vec3(x, y + sizeY, 0)));
}
void GameMonopoly::initActions() {

}

void GameMonopoly::init(int nbJoueur) {
	this->nbJoueur = nbJoueur;
	this->idJoueurActuel = 0;
	this->initZones();
	this->initUIZones();

	Ressource::addRessource("Argent");

	for (int i = 0; i < nbJoueur; i++) {
		this->joueurs.push_back(Joueur(zones[0]));
	}

	this->initActions();
}

void GameMonopoly::initGame() {
	for (int i = 0; i < nbJoueur; i++) {
		initGameAction.Play(this->joueurs[i]);
	}
}