#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "Zone.h"
#include "LoadFile.h"
#include "Camera.h"
#include "Shader.h"

class OpenGLContext;

class Viewer {
public:
	Viewer() {
		bViewSelectionZone = false;
	};
	void InitScene(OpenGLContext& gl);
	void RenderScene(OpenGLContext& gl, Camera& camera, int windowHeight, int windowWidth);
	void drawColorScene(OpenGLContext& gl);
	void drawScene3d(OpenGLContext& gl);
	void getSelection();
	void drawPlateau();

	int xMousePick;
	int yMousePick;

	bool bViewSelectionZone;

private:
	std::vector<Zone> Zones;
	Shader *shader;
	Shader *shaderColored;
	Shader *shaderTextured;

	int tPlateau;


	glm::mat4 projectionMatrix; // Matrice de projection
	glm::mat4 viewMatrix; // Matrice de vue
	glm::mat4 modelMatrix; // Matrice de modèle

	unsigned int vaoPlateau;
	unsigned int vboPlateau[2];

	unsigned int vaoZone;
	unsigned int vboZone[2];

	unsigned int vaoRect;
	unsigned int vboRect;
	
};