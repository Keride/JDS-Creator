#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "Couleur.h"


class Zone {
public:
	Zone() {};
	Zone(std::vector<glm::vec3> position);

private:
	std::vector<glm::vec3> position;
	Couleur color;
};