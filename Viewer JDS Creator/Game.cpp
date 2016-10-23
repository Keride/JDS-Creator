#include "Game.h"

Game::Game() {

}

void Game::initZones() {
	int nbZone = 0;
	float x = 1.0f;
	float z = 1.0f;
	float sizeX = 0.25f;
	float sizeZ = 0.25f;
	//-------
	zones.push_back(Zone(nbZone, glm::vec3(x, 0.0f, z), glm::vec3(x - sizeX, 0.0f, z), glm::vec3(x - sizeX, 0.0f, z - sizeZ), glm::vec3(x, 0.0f, z - sizeZ)));
	nbZone++;
	x -= sizeX;
	sizeX = 0.1666;
	for (int i = 0; i < 9; i++) {
		zones.push_back(Zone(nbZone, glm::vec3(x, 0.0f, z), glm::vec3(x - sizeX, 0.0f, z), glm::vec3(x - sizeX, 0.0f, z - sizeZ), glm::vec3(x, 0.0f, z - sizeZ)));
		nbZone++;
		x -= sizeX;
	}
	sizeX = 0.25f;
	sizeZ = 0.25f;
	zones.push_back(Zone(nbZone, glm::vec3(x, 0.0f, z), glm::vec3(x - sizeX, 0.0f, z), glm::vec3(x - sizeX, 0.0f, z - sizeZ), glm::vec3(x, 0.0f, z - sizeZ)));
	nbZone++;
	//-----
	z -= sizeZ;
	sizeZ = 0.1666;
	for (int i = 0; i < 9; i++) {
		zones.push_back(Zone(nbZone, glm::vec3(x, 0.0f, z), glm::vec3(x - sizeX, 0.0f, z), glm::vec3(x - sizeX, 0.0f, z - sizeZ), glm::vec3(x, 0.0f, z - sizeZ)));
		nbZone++;
		z -= sizeZ;
	}
	sizeX = 0.25f;
	sizeZ = 0.25f;
	zones.push_back(Zone(nbZone, glm::vec3(x, 0.0f, z), glm::vec3(x - sizeX, 0.0f, z), glm::vec3(x - sizeX, 0.0f, z - sizeZ), glm::vec3(x, 0.0f, z - sizeZ)));
	nbZone++;
	//-----
	sizeX = 0.1666;
	x += sizeX;
	for (int i = 0; i < 9; i++) {
		zones.push_back(Zone(nbZone, glm::vec3(x, 0.0f, z), glm::vec3(x - sizeX, 0.0f, z), glm::vec3(x - sizeX, 0.0f, z - sizeZ), glm::vec3(x, 0.0f, z - sizeZ)));
		nbZone++;
		x += sizeX;
	}
	x += 0.25 - sizeX;
	sizeX = 0.25f;
	sizeZ = 0.25f;
	zones.push_back(Zone(nbZone, glm::vec3(x, 0.0f, z), glm::vec3(x - sizeX, 0.0f, z), glm::vec3(x - sizeX, 0.0f, z - sizeZ), glm::vec3(x, 0.0f, z - sizeZ)));
	nbZone++;
	//-----
	sizeZ = 0.1666;
	z += sizeZ;
	for (int i = 0; i < 9; i++) {
		zones.push_back(Zone(nbZone, glm::vec3(x, 0.0f, z), glm::vec3(x - sizeX, 0.0f, z), glm::vec3(x - sizeX, 0.0f, z - sizeZ), glm::vec3(x, 0.0f, z - sizeZ)));
		nbZone++;
		z += sizeZ;
	}
}
void Game::init(int nbJoueur) {
	this->initZones();
	for (int i = 0; i < nbJoueur; i++) {
		this->joueurs.push_back(Joueur(&zones[0]));
	}
}

std::vector<Zone> Game::getZones() {
	return this->zones;
}
std::vector<Joueur> Game::getJoueurs() {
	return this->joueurs;
}
std::vector<Objet> Game::getObjets() {
	return this->objets;
}