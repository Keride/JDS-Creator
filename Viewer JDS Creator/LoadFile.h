#pragma once

#include <libpng\png.h>

#include <glew.h>
#include <wglew.h>
#include <glm/glm.hpp>


GLuint loadPNGTexture(const char *filename, int * width, int * height);
GLuint loadBMP(const char * filename);

struct Texture {
	int id;
	int height;
	int width;

	void Load(const char * fname);
	void Select();
};

struct Textures {
	Texture texture;
	Textures() {};
	void LoadTextures();
};
