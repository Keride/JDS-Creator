#pragma once
#include "Zone.h"
#include "Action.h"

class Bouton : public Zone{
public:
	Bouton();
	Bouton(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, glm::vec3 v4);
	~Bouton();
	void setAction(Action* action);
	void onClick();
private: 
	Action* action;
};

