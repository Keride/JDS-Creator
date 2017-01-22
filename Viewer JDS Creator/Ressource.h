#pragma once
#include <map>
#include <vector>

class Ressource{
public:
	Ressource();
	int Add(int value);
	int getValue() const;

	static void addRessource(std::string name);
	static int getnbRessource();
	static int getRessourceId(std::string name);
	static std::string getRessourceName(int i);
	static void addTexture(int idTexture);
	static int getTexture(int idTexture);
private:
	int value;

	static std::vector<std::string> RessourceName;
	static std::vector<int> RessourceTexture;

};