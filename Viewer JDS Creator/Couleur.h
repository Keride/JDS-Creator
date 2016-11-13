#pragma once
#include <string>
#include <assert.h>
#include <glm/glm.hpp>

class Couleur {
public:
	Couleur() {};
	Couleur(float r, float g, float b, float a);
	void Set(float r, float g, float b, float a);
	void Set(int index);
	float* Get() const; 

private:
	float r;
	float g;
	float b;
	float a;
};

bool operator<(Couleur const &a, Couleur const& b);