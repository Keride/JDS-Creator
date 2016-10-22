#pragma once

class Couleur {
public:
	Couleur() {};
	Couleur(float r, float g, float b, float a);
	void Set(float r, float g, float b, float a);
private:
	float r;
	float g;
	float b;
	float a;
};