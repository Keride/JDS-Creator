#pragma once
#include "Joueur.h"
#include "Case.h"

class JoueurMonopoly : public Joueur{
public:

	JoueurMonopoly::JoueurMonopoly(Case* pCaseDepart);
	glm::vec3 getTranslation() const;
	Case* getCase() const;
	void setCase(Case* pNextCase);
	~JoueurMonopoly();
	int nextDeplacement;
	bool nextDeplacementIsDouble;
private:
	Case* pCurrentCase;
};

