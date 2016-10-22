#include "Couleur.h"

Couleur::Couleur(float r, float g, float b, float a) {
	this->Set(r, g, b, a);
};
void Couleur::Set(float r, float g, float b, float a) {
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;
}