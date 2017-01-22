#include "Objet.h"

Objet::Objet() {

}

Zone* Objet::getZone() {
	return this->pZone;
}
Objet::Objet(Zone* pZone) {
	this->pZone = pZone;
}