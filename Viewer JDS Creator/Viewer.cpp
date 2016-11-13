#include "Viewer.h"

void Viewer::SetGame(Game* pGame) {
	this->pGame = pGame;
}
void Viewer::setFrame(int nbframes) {
	this->frame = nbframes;
}
void Viewer::InitScene(OpenGLContext& gl) {
	Viewer& m = *this;
	m.shader = new Shader("shader.vert", "shader.frag"); // On crée le shader en chargeant les shaders de type vertex et fragment.
	m.shaderTextured = new Shader("shaderTextured.vert", "shaderTextured.frag"); // On crée le shader en chargeant les shaders de type vertex et fragment.
	m.shaderColored = new Shader("SimpleShader.vert", "SimpleShader.frag"); // On crée le shader en chargeant les shaders de type vertex et fragment.
	m.shaderText = new Shader("TextShader.vert", "TextShader.frag"); // On crée le shader en chargeant les shaders de type vertex et fragment.
	
	m.tPlateau = loadPNGTexture("./Assets/Monopoly/Monopoly.png", NULL, NULL);

	for (int i = 0; i < Ressource::getnbRessource(); i++) {
		std::string adress = "./Assets/Monopoly/Ressources/" + Ressource::getRessourceName(i) + ".png";
		Ressource::addTexture(loadPNGTexture(adress.c_str(), NULL, NULL));
	}
	gl.createTexturedRectangle(vaoPlateau, vboPlateau);

	gl.createUniqueQuad(vaoRect, vboRect);
	gl.createUniqueQuadTextured(vaoRectTextured, vboRectTextured);
	gl.createUniqueQuad3d(vaoRect3d, vboRect3d);
}

void Viewer::drawPlateau() {
	Viewer& m = *this;
	m.shaderTextured->bind(); // Lier le shader

	int projectionMatrixLocation = glGetUniformLocation(shaderTextured->id(), "projectionMatrix");
	int viewMatrixLocation = glGetUniformLocation(shaderTextured->id(), "viewMatrix");
	int modelMatrixLocation = glGetUniformLocation(shaderTextured->id(), "modelMatrix"); 

	glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
	glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]); 

	glBindTexture(GL_TEXTURE_2D, tPlateau);
	glBindVertexArray(vaoPlateau); // Lier le VAO
	glDrawArrays(GL_TRIANGLES, 0, 12); // Dessiner la grille
	glBindVertexArray(0); // Délier le VAO 

	glEnable(GL_DEPTH_TEST);
	m.shaderTextured->unbind();
}

void Viewer::drawUI(OpenGLContext& gl) {
	Viewer& m = *this;
	m.shaderTextured->bind(); // Lier le shader

	int projectionMatrixLocation = glGetUniformLocation(shaderTextured->id(), "projectionMatrix");
	int viewMatrixLocation = glGetUniformLocation(shaderTextured->id(), "viewMatrix");
	//m.modelMatrix = glm::rotate(m.modelMatrix, 1.0f, glm::vec3(0.0f, 0.0f, 1.0f));
	m.modelMatrix = glm::scale(m.modelMatrix, glm::vec3(0.08f));
	int modelMatrixLocation = glGetUniformLocation(shaderTextured->id(), "modelMatrix");

	glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &this->projectionMatrix[0][0]);
	glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &this->viewMatrix[0][0]);
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &this->modelMatrix[0][0]);

	glBindTexture(GL_TEXTURE_2D, Ressource::getTexture(0));
	glBindVertexArray(this->vaoRectTextured); // Lier le VAO
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);//(GL_TRIANGLES, 0, 12); // Dessiner la grille
	glBindVertexArray(0); // Délier le VAO 

	glEnable(GL_DEPTH_TEST);
	m.shaderTextured->unbind();

	float color[4] = { 0.0, 1.8f, 1.0f, 0.0f };

	char buffer[32];
	snprintf(buffer, sizeof(buffer), "%d $", m.pGame->getJoueurs()[0].getRessource(0));

	gl.RenderText(*m.shaderText, buffer, 70.0f, 570.0f, 0.5f, color);
}

void Viewer::drawColorUI(OpenGLContext& gl){
	Viewer& m = *this;
	m.shaderColored->bind();
	int ColorLocation = glGetUniformLocation(shaderColored->id(), "Color");
	int projectionMatrixLocation = glGetUniformLocation(shaderColored->id(), "projectionMatrix");
	int viewMatrixLocation = glGetUniformLocation(shaderColored->id(), "viewMatrix");
	int modelMatrixLocation = glGetUniformLocation(shaderColored->id(), "modelMatrix");

	glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
	glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);
	const std::vector<Zone*>& zones = pGame->getUIZones();
	for (int i = 0; i < zones.size(); i++) {
		const Zone& zone = *zones[i];
		glUniform4fv(ColorLocation, 1, zone.getColor());
		glm::mat4 model = glm::translate(modelMatrix, zone.getTranslation());
		model = glm::scale(model, zone.getScale());
		glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &model[0][0]);
		glBindVertexArray(vaoRect);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
	m.shaderColored->unbind();
}

void Viewer::drawColorScene(OpenGLContext& gl) {
	Viewer& m = *this;
	m.shaderColored->bind();
	int ColorLocation = glGetUniformLocation(shaderColored->id(), "Color");
	int projectionMatrixLocation = glGetUniformLocation(shaderColored->id(), "projectionMatrix");
	int viewMatrixLocation = glGetUniformLocation(shaderColored->id(), "viewMatrix");
	int modelMatrixLocation = glGetUniformLocation(shaderColored->id(), "modelMatrix"); 

	glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
	glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);
	const std::vector<Zone*>& zones = pGame->getZones();
	for (int i = 0; i < zones.size(); i++) {
		const Zone& zone = *zones[i];
		glUniform4fv(ColorLocation, 1, zone.getColor());
		glm::mat4 model = glm::translate(modelMatrix, zone.getTranslation());
		model = glm::scale(model, zone.getScale());
		glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &model[0][0]);
		glBindVertexArray(vaoRect);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
	m.shaderColored->unbind();
}

void Viewer::getSelection() {
	Viewer& m = *this;
	unsigned char res[4];
	res[0] = 0;
	GLint viewport[4];

	glGetIntegerv(GL_VIEWPORT, viewport);
	glReadPixels(this->xMousePick, viewport[3] - this->yMousePick, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &res);
	if (res[0] != 0) {
		Zone* zone = Zone::getZone(res[0], res[1], res[2], res[3]);
		zone->onClick();
	}

}


void Viewer::drawScene3d(OpenGLContext& gl) {
	Viewer& m = *this;
	m.drawPlateau();
	m.drawJoueurs();
}

void Viewer::drawJoueurs() {
	Viewer& m = *this;

	m.shaderColored->bind();
	int ColorLocation = glGetUniformLocation(shaderColored->id(), "Color");
	int projectionMatrixLocation = glGetUniformLocation(shaderColored->id(), "projectionMatrix");
	int viewMatrixLocation = glGetUniformLocation(shaderColored->id(), "viewMatrix");
	int modelMatrixLocation = glGetUniformLocation(shaderColored->id(), "modelMatrix");

	glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
	glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]);

	const std::vector<Joueur>& joueurs = pGame->getJoueurs();
	for (int i = 0; i < joueurs.size(); i++) {
		const Joueur& j = joueurs[i];
		GLfloat color[] = { 1.0f,0.0f,0.0f,1.0f };
		glUniform4fv(ColorLocation, 1, color); //j.getColor());
		glm::mat4 model = glm::translate(modelMatrix, j.getTranslation());
		model = glm::scale(model, glm::vec3(0.05f));
		glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &model[0][0]);
		glBindVertexArray(vaoRect3d);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
	m.shaderColored->unbind();
}
void Viewer::RenderScene(OpenGLContext& gl, Camera& camera, int windowHeight, int windowWidth) {
	Viewer& m = *this;

	this->projectionMatrix = glm::perspective(camera.Zoom, (float)windowWidth / (float)windowHeight, 0.1f, 100.f);
	this->viewMatrix = camera.GetViewMatrix();
	this->modelMatrix = glm::mat4(1.0f);

	drawColorScene(gl);

	this->projectionMatrix = glm::ortho(0.0f, 1.0f, 1.0f, 0.0f, -10.0f, 1000.0f);
	this->viewMatrix = glm::mat4(1.0f);
	this->modelMatrix = glm::mat4(1.0f);
	drawColorUI(gl);

	getSelection();
	
	if(bViewSelectionZone)
		return;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); // On vide les tampons. 

	this->projectionMatrix = glm::perspective(camera.Zoom, (float)windowWidth / (float)windowHeight, 0.1f, 100.f);
	this->viewMatrix = camera.GetViewMatrix();
	drawScene3d(gl);

	float color[4] = { 1.0, 1.8f, 1.0f, 0.0f };

	//printf("%f ms/frame\n", 1000.0 / double(nbFrames));

	char buffer[32];
	snprintf(buffer, sizeof(buffer), "%g ms", (1000.0 / double(this->frame)));

	gl.RenderText(*m.shaderText, buffer, 700.0f, 550.0f, 0.3f, color); 
	this->projectionMatrix = glm::ortho(0.0f, 1.0f, 1.0f, 0.0f, -10.0f, 1000.0f);
	//this->projectionMatrix = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, -10.0f, 1000.0f);
	this->viewMatrix = glm::mat4(1.0f);
	//this->modelMatrix = glm::translate(modelMatrix, glm::vec3(1.0f, 0.0f, 0.0f));
	drawUI(gl);

}