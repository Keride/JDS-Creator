#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "Zone.h"
#include "LoadFile.h"
#include "Camera.h"
#include "Shader.h"
#include "opengl_3.h"
#include "Game.h"

class OpenGLContext;

class Viewer {
public:
	Viewer() {
		bViewSelectionZone = false;
	};

	void SetGame(Game* pGame);
	void setFrame(int nbframes);
	void InitScene(OpenGLContext& gl);
	void RenderScene(OpenGLContext& gl, Camera& camera, int windowHeight, int windowWidth);
	void drawColorScene(OpenGLContext& gl);
	void drawScene3d(OpenGLContext& gl);
	void getSelection();
	void drawPlateau();
	void drawJoueurs();

	int xMousePick;
	int yMousePick;

	bool bViewSelectionZone;

private:
	Game* pGame;

	Shader *shader;
	Shader *shaderColored;
	Shader *shaderTextured; 
	Shader *shaderText;

	int tPlateau;
	int frame;

	glm::mat4 projectionMatrix; // Matrice de projection
	glm::mat4 viewMatrix; // Matrice de vue
	glm::mat4 modelMatrix; // Matrice de modèle

	unsigned int vaoPlateau;
	unsigned int vboPlateau[2];

	unsigned int vaoRect;
	unsigned int vboRect;

	unsigned int vaoRect3d;
	unsigned int vboRect3d;
	
};