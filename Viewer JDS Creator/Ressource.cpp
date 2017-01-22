#include "Ressource.h"

Ressource::Ressource() {
	this->value = 0;
}

int Ressource::Add(int value)
{
	this->value += value;
	return this->value;
}

int Ressource::getValue() const {
	return this->value;
}

std::vector<std::string> Ressource::RessourceName;
std::vector<int> Ressource::RessourceTexture;

int Ressource::getnbRessource() {
	return RessourceName.size();
}
void Ressource::addRessource(std::string name) {
	RessourceName.push_back(name);
}
int Ressource::getRessourceId(std::string name) {
	for (int i = 0; i < RessourceName.size(); i++) {
		if (name == RessourceName[i])
			return i;
	}
	return -1;
}
std::string Ressource::getRessourceName(int i) {
	return RessourceName[i];
}

void Ressource::addTexture(int idTexture) {
	RessourceTexture.push_back(idTexture);
}
int Ressource::getTexture(int idTexture) {
	return RessourceTexture[idTexture];
}