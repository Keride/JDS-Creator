#include "Shader.h"
#include <string.h>
#include <iostream>
#include <stdlib.h>


using namespace std;

static char* textFileRead(const char *fileName) {
	char* text = NULL;

	if (fileName != NULL) {
		FILE *file;
		fopen_s(&file,fileName, "rt");

		if (file != NULL) {
			fseek(file, 0, SEEK_END);
			int count = ftell(file);
			rewind(file);

			if (count > 0) {
				text = (char*)malloc(sizeof(char) * (count + 1));
				count = fread(text, sizeof(char), count, file);
				text[count] = '\0';
			}
			fclose(file);
		}
	}
	return text;
}

Shader::Shader() {

}

Shader::Shader(const char *vsFile, const char *fsFile) {
	init(vsFile, fsFile);
}

void Shader::init(const char *vsFile, const char *fsFile) {
	shader_vp = glCreateShader(GL_VERTEX_SHADER);
	shader_fp = glCreateShader(GL_FRAGMENT_SHADER);

	const char* vsText = textFileRead(vsFile);
	const char* fsText = textFileRead(fsFile);

	//glBindAttribLocation(shader_id, 0, "in_Position"); // Lier un attribut constant pour les positions des arêtes. 
	//glBindAttribLocation(shader_id, 1, "in_Color"); //  Lier un attribut constant pour la couleur. 

	if (vsText == NULL || fsText == NULL) {
		cerr << "Either vertex shader or fragment shader file not found." << endl;
		return;
	}

	glShaderSource(shader_vp, 1, &vsText, 0); 
	glShaderSource(shader_fp, 1, &fsText, 0); 

	glCompileShader(shader_vp);
	glCompileShader(shader_fp);

	int statCompilation;
	glGetShaderiv(shader_vp, GL_COMPILE_STATUS, &statCompilation);

	if (statCompilation != true) {
		GLint infoLogLength;
		glGetShaderiv(shader_vp, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar* infoLog = new GLchar[infoLogLength + 1];
		glGetShaderInfoLog(shader_vp, infoLogLength, &statCompilation, &infoLog[0]);
		glDeleteShader(shader_vp);
		throw std::runtime_error(infoLog);
	}

	glGetShaderiv(shader_fp, GL_COMPILE_STATUS, &statCompilation);

	if (statCompilation != true) {
		glGetShaderiv(shader_fp, GL_INFO_LOG_LENGTH, &statCompilation);
		std::string log(statCompilation, 0);
		glGetShaderInfoLog(shader_fp, statCompilation, &statCompilation, &log[0]);
		glDeleteShader(shader_fp);
		throw std::runtime_error(log);
	}
	shader_id = glCreateProgram();

	glAttachShader(shader_id, shader_fp);
	glAttachShader(shader_id, shader_vp);
	glLinkProgram(shader_id);
	int statLink;

	glGetProgramiv(shader_id, GL_LINK_STATUS, &statLink);

	if (statLink != true) {
		glGetProgramiv(shader_id, GL_INFO_LOG_LENGTH, &statLink);
		std::string log(statLink, 0);
		glGetProgramInfoLog(shader_id, statLink, &statLink, &log[0]);
		throw std::runtime_error(log);
	}
}

Shader::~Shader() {
	glDetachShader(shader_id, shader_fp);
	glDetachShader(shader_id, shader_vp);

	glDeleteShader(shader_fp);
	glDeleteShader(shader_vp);
	glDeleteProgram(shader_id);
}

unsigned int Shader::id() {
	return shader_id;
}

void Shader::bind() {
	glUseProgram(shader_id);
}

void Shader::unbind() {
	glUseProgram(0);
}
