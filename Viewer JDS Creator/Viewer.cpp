#include "Viewer.h"

void Viewer::SetGame(Game* pGame) {
	this->pGame = pGame;
}
void Viewer::setFrame(int nbframes) {
	this->frame = nbframes;
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
	glDrawArrays(GL_TRIANGLES, 0, 6);
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0); // Délier le VAO 

	glEnable(GL_DEPTH_TEST);
	m.shaderTextured->unbind();
}

void Viewer::InitScene(OpenGLContext& gl) {
	Viewer& m = *this;
	m.shaderTextured = new Shader("Shader/shaderTextured.vert", "Shader/shaderTextured.frag"); // On crée le shader en chargeant les shaders de type vertex et fragment.
	m.shaderColored = new Shader("Shader/SimpleShader.vert", "Shader/SimpleShader.frag"); // On crée le shader en chargeant les shaders de type vertex et fragment.
	m.shaderText = new Shader("Shader/TextShader.vert", "Shader/TextShader.frag"); // On crée le shader en chargeant les shaders de type vertex et fragment.
	
	m.tPlateau = loadPNGTexture("./Assets/Monopoly/Monopoly.png", NULL, NULL);

	for (int i = 0; i < Ressource::getnbRessource(); i++) {
		std::string adress = "./Assets/Monopoly/Ressources/" + Ressource::getRessourceName(i) + ".png";
		Ressource::addTexture(loadPNGTexture(adress.c_str(), NULL, NULL));
	}
	for (int i = 0; i < m.pGame->getActions().size(); i++) {
		Action* action = m.pGame->getActions()[i];
		action->setTextureId(loadPNGTexture(action->getTexture(), NULL, NULL));
	}
	gl.createTexturedRectangle(vaoPlateau, vboPlateau);

	gl.createUniqueQuad(vaoRect, vboRect);
	gl.createUniqueQuadTextured(vaoRectTextured, vboRectTextured);
	gl.createUniqueQuad3d(vaoRect3d, vboRect3d);
}

void Viewer::drawUI(OpenGLContext& gl, Camera& camera) {
	Viewer& m = *this;


	{	//Rect Rouge en haut de l'ecran
		m.shaderColored->bind();
		int ColorLocation = glGetUniformLocation(shaderColored->id(), "Color");
		int projectionMatrixLocation = glGetUniformLocation(shaderColored->id(), "projectionMatrix");
		int viewMatrixLocation = glGetUniformLocation(shaderColored->id(), "viewMatrix");
		int modelMatrixLocation = glGetUniformLocation(shaderColored->id(), "modelMatrix");

		m.modelMatrix = glm::mat4(1.0f);
		glm::mat4 model = glm::translate(modelMatrix, glm::vec3(0.0f, 0.0f, -0.5f));
		model = glm::scale(model, glm::vec3(1.0f, 0.1f, 0.03f) / camera.Zoom);
		GLfloat color[] = { 1.0f,0.0f,0.0f, 1.0f };

		glUniform4fv(ColorLocation, 1, color);
		glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &this->projectionMatrix[0][0]);
		glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &this->viewMatrix[0][0]);
		glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &model[0][0]);

		glBindVertexArray(vaoRect);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		m.shaderColored->unbind();
	}	
	{	//Actions en bas de l'ecran
		m.shaderTextured->bind(); 

		int projectionMatrixLocation = glGetUniformLocation(shaderTextured->id(), "projectionMatrix");
		int viewMatrixLocation = glGetUniformLocation(shaderTextured->id(), "viewMatrix");
		int modelMatrixLocation = glGetUniformLocation(shaderTextured->id(), "modelMatrix");

		glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &this->projectionMatrix[0][0]);
		glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &this->viewMatrix[0][0]);
		glm::mat4 model = glm::scale(m.modelMatrix, glm::vec3(0.08f) / camera.Zoom);
		glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &model[0][0]);

		glBindTexture(GL_TEXTURE_2D, Ressource::getTexture(0));
		glBindVertexArray(this->vaoRectTextured); 
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0); 

		m.modelMatrix = glm::mat4(1.0f);
		const std::vector<Action*>& actions = pGame->getNextPossibleActions();
		for (size_t i = 0; i < actions.size(); i++) {
			const Action& action = *actions[i];
			const Bouton& zone = *action.getBouton();
			glm::mat4 model = glm::translate(modelMatrix, zone.getTranslation() / camera.Zoom);
			model = glm::scale(model, glm::vec3(0.08f)/ camera.Zoom);
			glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &model[0][0]);
			glBindTexture(GL_TEXTURE_2D, action.getIdTexture());
			glBindVertexArray(this->vaoRectTextured);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}

		glEnable(GL_DEPTH_TEST);
		m.shaderTextured->unbind();
	}

	{	//Ressource
		glDisable(GL_DEPTH_TEST);
		float color[4] = { 0.0, 1.8f, 1.0f, 1.0f };

		char buffer[32];
		snprintf(buffer, sizeof(buffer), "%d $", m.pGame->getJoueurs()[0]->getRessource(0).getValue());
		gl.RenderText(*m.shaderText, buffer, 70.0f, 570.0f, 0.5f, color);

		if (((JoueurMonopoly*)m.pGame->getJoueurs()[0])->nextDeplacement) {
			snprintf(buffer, sizeof(buffer), "%d", ((JoueurMonopoly*)m.pGame->getJoueurs()[0])->nextDeplacement);
			gl.RenderText(*m.shaderText, buffer, 350.0f, 570.0f, 0.8f, color);
		}
		glEnable(GL_DEPTH_TEST);
	}
	
}

void Viewer::drawColorUI(OpenGLContext& gl, Camera& camera){
	Viewer& m = *this;
	m.shaderColored->bind();
	int ColorLocation = glGetUniformLocation(shaderColored->id(), "Color");
	int projectionMatrixLocation = glGetUniformLocation(shaderColored->id(), "projectionMatrix");
	int viewMatrixLocation = glGetUniformLocation(shaderColored->id(), "viewMatrix");
	int modelMatrixLocation = glGetUniformLocation(shaderColored->id(), "modelMatrix");

	glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
	glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);
	const std::vector<Action*>& actions = pGame->getNextPossibleActions();
	for (size_t i = 0; i < actions.size(); i++) {
		const Action& action = *actions[i];
		const Bouton& zone = *action.getBouton();
		glUniform4fv(ColorLocation, 1, zone.getColor());
		glm::mat4 model = glm::translate(modelMatrix, zone.getTranslation() / camera.Zoom);
		model = glm::scale(model, zone.getScale() / camera.Zoom);
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
	for (size_t i = 0; i < zones.size(); i++) {
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
		zone->onClick(pGame->getJoueurActuel());
		this->xMousePick = -1;
		this->yMousePick = -1;
		pGame->updateActions(((Bouton*)zone)->getAction());
	}

}


void Viewer::drawScene3d(OpenGLContext& gl) {
	Viewer& m = *this;
	m.drawPlateau();
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.05f, 0.0f));
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

	const std::vector<Joueur*>& joueurs = pGame->getJoueurs();
	for (size_t i = 0; i < joueurs.size(); i++) {
		const Joueur* j = joueurs[i];
		GLfloat color[] = { 1.0f,0.0f,0.0f,1.0f };
		glUniform4fv(ColorLocation, 1, color); //j.getColor());
		glm::mat4 model = glm::translate(modelMatrix, ((JoueurMonopoly*)j)->getTranslation());
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

	this->projectionMatrix = glm::ortho(0.0f/ camera.Zoom, 1.0f / camera.Zoom, 1.0f / camera.Zoom, 0.0f / camera.Zoom, -10.0f, 1000.0f);
	this->viewMatrix = camera.GetViewMatrix();
	this->modelMatrix = glm::mat4(1.0f);

	drawColorScene(gl);
	this->viewMatrix = glm::mat4(1.0f);
	this->modelMatrix = glm::mat4(1.0f);
	drawColorUI(gl, camera);
	getSelection();
	
	if(bViewSelectionZone)
		return;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); // On vide les tampons. 
	this->viewMatrix = camera.GetViewMatrix();
	drawScene3d(gl);

	float color[4] = { 1.0, 1.8f, 1.0f, 0.0f };
	char buffer[32];
	snprintf(buffer, sizeof(buffer), "%g ms", (1000.0 / double(this->frame)));
	gl.RenderText(*m.shaderText, buffer, 700.0f, 550.0f, 0.3f, color); 

	this->viewMatrix = glm::mat4(1.0f);
	//this->viewMatrix = glm::translate(viewMatrix, glm::vec3(0.0f, 0.0f, 10.0f));
	drawUI(gl, camera);

}