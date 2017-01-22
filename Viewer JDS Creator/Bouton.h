#pragma once
#include "Zone.h"


class Action;

class Bouton : public Zone{
public:
	Bouton();
	Bouton(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, glm::vec3 v4);
	~Bouton();
	void setAction(Action* action);
	Action* getAction() const;
	void onClick(Joueur* j);
private: 
	Action* action;
};

