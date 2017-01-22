#pragma once
#include "Zone.h"

class Objet {
public:
	Objet();
	Objet(Zone* pZone);
	Zone* getZone();
private:
	Zone* pZone;
};