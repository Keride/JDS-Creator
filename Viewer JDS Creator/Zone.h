#pragma once
#include <vector>
#include <map>
#include <glm/glm.hpp>
#include "Couleur.h"

class Joueur;

class Zone {
	static std::map<Couleur, Zone*> zones;
public:
	Zone() {};
	Zone(std::vector<glm::vec3> position);
	Zone(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, glm::vec3 v4); 
	float* getColor() const; 

	glm::vec3 getTranslation() const;
	glm::vec3 getScale() const;
	glm::vec3 getCenter() const;

	virtual void onClick(Joueur* j) {}

	static Zone* getZone(Couleur couleur);
	static Zone* getZone(int r, int g, int b, int a);

private:
	std::vector<glm::vec3> position;
	Couleur color;
};