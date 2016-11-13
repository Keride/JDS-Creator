#include "Zone.h"

static int nbZone = 0;
std::map<Couleur, Zone*> Zone::zones;
Zone::Zone(std::vector<glm::vec3> position) {
	Zone& m = *this;
	m.position = position;
	m.color.Set(nbZone++);
	Zone::zones[m.color] = this;
}

Zone::Zone(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, glm::vec3 v4) {
	Zone& m = *this;
	m.position.push_back(v1);
	m.position.push_back(v2);
	m.position.push_back(v3);
	m.position.push_back(v4);
	m.color.Set(nbZone++);
	Zone::zones[m.color] = this;
}

glm::vec3 Zone::getCenter() const {
	glm::vec3 center;
	for (int i = 0; i < this->position.size(); i++)
		center += position[i];
	center /= this->position.size();
	return center;
}
glm::vec3 Zone::getTranslation() const{ //assume rectangle
	return this->position[0];
}
glm::vec3 Zone::getScale() const{ //assume rectangle
	return glm::vec3(this->position[2][0] - this->position[0][0], this->position[2][1] - this->position[0][1], this->position[2][2] - this->position[0][2]);
}

float* Zone::getColor() const{
	return this->color.Get();
}

Zone* Zone::getZone(Couleur couleur) {
	return Zone::zones[couleur];
}
Zone* Zone::getZone(int r, int g, int b, int a) {
	Couleur couleur(r, g, b, a);
	return Zone::getZone(couleur);
}