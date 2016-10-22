#include "Viewer.h"
#include "opengl_3.h"

void Viewer::InitScene(OpenGLContext& gl) {
	Viewer& m = *this;
	m.shader = new Shader("shader.vert", "shader.frag"); // On crée le shader en chargeant les shaders de type vertex et fragment.
	m.shaderTextured = new Shader("shaderTextured.vert", "shaderTextured.frag"); // On crée le shader en chargeant les shaders de type vertex et fragment.
	m.shaderColored = new Shader("SimpleShader.vert", "SimpleShader.frag"); // On crée le shader en chargeant les shaders de type vertex et fragment.
	
	m.tPlateau = loadPNGTexture("./Assets/Monopoly/Monopoly.png", NULL, NULL);

	gl.createTexturedRectangle(vaoPlateau, vboPlateau);

	gl.createUniqueQuad(vaoRect, vboRect);
}

void Viewer::drawPlateau() {
	Viewer& m = *this;
	int modelMatrixLocation = glGetUniformLocation(shaderTextured->id(), "modelMatrix"); // Récupérer la localisation de la matrice de modèle dans le shader
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]); // Envoyer la matrice de modèle au shader

	glBindTexture(GL_TEXTURE_2D, tPlateau);
	glBindVertexArray(vaoPlateau); // Lier le VAO
	glDrawArrays(GL_TRIANGLES, 0, 12); // Dessiner la grille
	glBindVertexArray(0); // Délier le VAO 

	glEnable(GL_DEPTH_TEST);
}


void Viewer::drawColorScene(OpenGLContext& gl) {
	Viewer& m = *this;
	m.shaderColored->bind();
	GLfloat color[] = { 1.0f, 0.0f, 0.0f, 0.5f };
	int ColorLocation = glGetUniformLocation(shaderColored->id(), "Color");
	glUniform4fv(ColorLocation, 1, color);

	int projectionMatrixLocation = glGetUniformLocation(shaderColored->id(), "projectionMatrix");
	glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);

	int viewMatrixLocation = glGetUniformLocation(shaderColored->id(), "viewMatrix");
	glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);

	//glm::mat4 model = glm::scale(modelMatrix, glm::vec3(0.5, 1.0, 0.5));
	int modelMatrixLocation = glGetUniformLocation(shaderColored->id(), "modelMatrix"); // Récupérer la localisation de la matrice de modèle dans le shader
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]); // Envoyer la matrice de modèle au shader

	glBindVertexArray(vaoRect);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	GLfloat color2[] = { 0.0f, 0.0f, 1.0f, 0.5f };
	ColorLocation = glGetUniformLocation(shaderColored->id(), "Color");
	glUniform4fv(ColorLocation, 1, color2);

	glm::mat4 model = glm::translate(modelMatrix, glm::vec3(-0.5, 0.0, -0.5));
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &model[0][0]); // Envoyer la matrice de modèle au shader

	glBindVertexArray(vaoRect);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	m.shaderColored->unbind();
}

void Viewer::getSelection() {
	Viewer& m = *this;
	unsigned char res[4];
	res[0] = 0;
	GLint viewport[4];

	glGetIntegerv(GL_VIEWPORT, viewport);
	glReadPixels(this->xMousePick, viewport[3] - this->yMousePick, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &res);
	if (res[0] != 0)
		int a = 5;

}


void Viewer::drawScene3d(OpenGLContext& gl) {
	Viewer& m = *this;
	m.shaderTextured->bind(); // Lier le shader

	int projectionMatrixLocation = glGetUniformLocation(shaderTextured->id(), "projectionMatrix");
	int viewMatrixLocation = glGetUniformLocation(shaderColored->id(), "viewMatrix");
	int modelMatrixLocation = glGetUniformLocation(shaderTextured->id(), "modelMatrix"); // Récupérer la localisation de la matrice de modèle dans le shader
	//modelMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(2.0f));
	glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
	glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);

	m.drawPlateau();
	m.shaderTextured->unbind();

}

void Viewer::RenderScene(OpenGLContext& gl, Camera& camera, int windowHeight, int windowWidth) {
	Viewer& m = *this;

	projectionMatrix = glm::perspective(camera.Zoom, (float)windowWidth / (float)windowHeight, 0.1f, 100.f);
	viewMatrix = camera.GetViewMatrix();
	modelMatrix = glm::mat4(1.0f);

	drawColorScene(gl);
	getSelection();
	
	if(bViewSelectionZone)
		return;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); // On vide les tampons. 

	drawScene3d(gl);



}