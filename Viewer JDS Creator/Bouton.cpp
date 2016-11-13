#include "Bouton.h"



Bouton::Bouton(){
}


Bouton::Bouton(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, glm::vec3 v4) : Zone(v1,v2,v3,v4) {
	
}

void Bouton::setAction(Action* action) {
	this->action = action;
}
Bouton::~Bouton(){
}

void Bouton::onClick() {
	int a = 0;
}