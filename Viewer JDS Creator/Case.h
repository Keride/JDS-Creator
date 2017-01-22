#pragma once
#include "Objet.h"
#include "Zone.h"

class Case :public Objet{
public:
	Case();
	Case(Zone* pZone);
	void SetNextCase(Case* pNextCase);
	Case* getNextCase();
	~Case();

private:
	Case* pNextCase;
};

