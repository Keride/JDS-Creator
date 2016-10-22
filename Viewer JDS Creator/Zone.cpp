#include "Zone.h"

Zone::Zone(std::vector<glm::vec3> position) {
	Zone& m = *this;
	m.position = position;
}