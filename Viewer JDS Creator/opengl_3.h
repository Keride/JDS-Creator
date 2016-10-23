#pragma once
#include "main.h"
#include "Viewer.h"

class Viewer;

static void check_gl_error() {
	GLenum err(glGetError());

	while (err != GL_NO_ERROR) {
		std::string error;

		switch (err) {
			case GL_INVALID_OPERATION:      error = "INVALID_OPERATION";      break;
			case GL_INVALID_ENUM:           error = "INVALID_ENUM";           break;
			case GL_INVALID_VALUE:          error = "INVALID_VALUE";          break;
			case GL_OUT_OF_MEMORY:          error = "OUT_OF_MEMORY";          break;
			case GL_INVALID_FRAMEBUFFER_OPERATION:  error = "INVALID_FRAMEBUFFER_OPERATION";  break;
		}
		/*char msgbuf[100];
		sprintf_s(msgbuf, "GL_ %s - %s %d", error.c_str(), file, line);
		OutputDebugStringA(msgbuf);*/
		//std::cerr << "GL_" << error.c_str() << " - " << file << ":" << line << std::endl;
		err = glGetError();
	}
}
class OpenGLContext {
public:
	OpenGLContext(void); // Constructeur par défaut
	OpenGLContext(HWND hwnd); // Constructeur du contexte à partir d'un identifiant de fenêtre
	~OpenGLContext(void); // Destructeur pour le nettoyage de l'application
	bool create30Context(HWND hwnd); // Création d'un contexte OpenGL 3.x
	void setupScene(Viewer& v); // Définition de toutes les informations de scène
	void reshapeWindow(int w, int h); // Nouvelles dimensions au redimensionnement
	void renderScene(Camera& camera, Viewer& v); // Affichage de la scène
	void createConvexPolygon(float* vertices, int verticesCount, float* couleur, unsigned int& vao, unsigned int* vbo);

	void createUniqueQuad(unsigned int& vao, unsigned int& vbo);
	void createUniqueQuad3d(unsigned int& vao, unsigned int& vbo);

	void createSquare(void); // Méthode pour créer les VAO du carré
	void createTexturedSquare(void); // Méthode pour créer les VAO du carré
	void createGrid(void); // Méthode pour créer les VAO d'une grille
	void createTexturedRectangle(unsigned int& rectangleTexturedVAO, unsigned int* rectangleTextureVBO);
	void createAxis(void);

static void check_gl_error() {
	GLenum err(glGetError());

	while (err != GL_NO_ERROR) {
		std::string error;
		//assert(0);
		switch (err) {
			case GL_INVALID_OPERATION:      error = "INVALID_OPERATION";      break;
			case GL_INVALID_ENUM:           error = "INVALID_ENUM";           break;
			case GL_INVALID_VALUE:          error = "INVALID_VALUE";          break;
			case GL_OUT_OF_MEMORY:          error = "OUT_OF_MEMORY";          break;
			case GL_INVALID_FRAMEBUFFER_OPERATION:  error = "INVALID_FRAMEBUFFER_OPERATION";  break;
		}
		/*char msgbuf[100];
		sprintf_s(msgbuf, "GL_ %s - %s %d", error.c_str(), file, line);
		OutputDebugStringA(msgbuf);*/
		//std::cerr << "GL_" << error.c_str() << " - " << file << ":" << line << std::endl;
		err = glGetError();
	}
}


private:
	int windowWidth;  // Largeur de la fenêtre
	int windowHeight; // Hauteur de la fenêtre

	unsigned int vaoID; // VAO	
	unsigned int vboID[2]; // VBO

	unsigned int cubetexturedVAO; // VBO
	unsigned int cubetexturedVBO[2]; // VBO

	unsigned int rectangleTexturedVAO;
	unsigned int rectangleTexturedVBO[2]; // VBO

	GLuint texId;
	unsigned int gridVAO; // VAO
	unsigned int axisVAO; // VAO
	unsigned int gridVBO; // VBO
	unsigned int axisVBO[2]; // VBO



protected:
	HGLRC hrc; // Contexte de rendu
	HDC hdc;   // Contexte matériel
	HWND hwnd; // Identifiant de la fenêtre
};