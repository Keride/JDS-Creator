#include "Bouton.h"
#include "Action.h"


Bouton::Bouton(){
}


Bouton::Bouton(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, glm::vec3 v4) : Zone(v1,v2,v3,v4) {

}
Action* Bouton::getAction() const {
	return this->action;
}
Bouton::~Bouton(){
}

void Bouton::setAction(Action* action) {
	this->action = action;
}
void Bouton::onClick(Joueur* j) {
	this->action->Play(j);
}