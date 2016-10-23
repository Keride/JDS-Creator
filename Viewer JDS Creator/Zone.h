#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "Couleur.h"


class Zone {
public:
	Zone() {};
	Zone(int index, std::vector<glm::vec3> position);
	Zone(int index, glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, glm::vec3 v4); 
	float* getColor() const; 
	glm::vec3 getTranslation() const;
	glm::vec3 getScale() const;
	glm::vec3 getCenter() const;

private:
	std::vector<glm::vec3> position;
	Couleur color;
};