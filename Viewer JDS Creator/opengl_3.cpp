#include "opengl_3.h"
/**Constructeur par d�faut de la classe OpenGLContext. � pr�sent, il ne fait rien,
mais on peut y ajouter ce que l'on veut. */
OpenGLContext::OpenGLContext(void) {
}

/**Constructeur de la classe OpenGLContext qui cr�e un contexte avec un HWND.*/
OpenGLContext::OpenGLContext(HWND hwnd) {
	create30Context(hwnd);
}

/**Destructeur de la classe OpenGLContext, qui va nettoyer le contexte de rendu et
lib�rer le contexte mat�riel de la fen�tre courante.*/
OpenGLContext::~OpenGLContext(void) {
	wglMakeCurrent(hdc, 0); // Enlever le contexte de rendu du contexte mat�riel
	wglDeleteContext(hrc); // Supprimer le contexte de rendu
	ReleaseDC(hwnd, hdc); // Lib�rer le contexte mat�riel de la fen�tre
}

/**create30Context cr�e un contexte OpenGL et l'attache � la fen�tre point�e par le HWND.
Cette m�thode cr�e, pour l'instant, un contexte OpenGL 3.2 par d�faut, mais tentera
de cr�er un contexte OpenGL 2.1 si cela n'est pas possible.*/
bool OpenGLContext::create30Context(HWND hwnd) {
	this->hwnd = hwnd; // On d�finit le HWND de la fen�tre
	hdc = GetDC(hwnd); // On r�cup�re le contexte mat�riel pour la fen�tre

	PIXELFORMATDESCRIPTOR pfd; // On cr�e un nouveau PIXELFORMATDESCRIPTOR (PFD)  
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR)); // que l'on efface
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR); // On en d�finit la taille � la taille de la structure
	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW; // On active le double buffering, le support d'OpenGL et le dessin sur une fen�tre
	pfd.iPixelType = PFD_TYPE_RGBA; // On utilise des pixels RVBA
	pfd.cColorBits = 32; // Trente-deux bits d'informations de couleur (plus cette valeur est �lev�e, plus on a de couleurs)
	pfd.cDepthBits = 32; // Trente-deux bits d'information de profondeur (plus cette valeur est �lev�e, plus on a de niveaux de d�tails)
	pfd.iLayerType = PFD_MAIN_PLANE; // On d�finit le calque du PFD

	int nPixelFormat = ChoosePixelFormat(hdc, &pfd); // V�rifie si le PFD est valide et renvoie un format de pixels
	if (nPixelFormat == 0) // Si elle �choue
		return false;

	bool bResult = SetPixelFormat(hdc, nPixelFormat, &pfd); // On essaie de d�finir le format de pixels
	if (!bResult) // Si elle �choue
		return false;

	HGLRC tempOpenGLContext = wglCreateContext(hdc); // On cr�e un contexte OpenGL 2.1 pour le contexte mat�riel
	wglMakeCurrent(hdc, tempOpenGLContext); // On rend ce contexte courant et actif

	glewExperimental = GL_TRUE;
	GLenum error = glewInit(); // On active GLEW  
	if (error != GLEW_OK) // Si cela �choue
		return false;

	int attributes[] = {
		WGL_CONTEXT_MAJOR_VERSION_ARB, 3, // Version majeure d'OpenGL : 3
		WGL_CONTEXT_MINOR_VERSION_ARB, 2, // Version mineure d'OpenGL : 2  
		WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB, // Compatibilit� ascendante
		0
	};

	if (wglewIsSupported("WGL_ARB_create_context") == 1) { // Si l'extension de cr�ation de contexte OpenGL 3.x est disponible
		hrc = wglCreateContextAttribsARB(hdc, NULL, attributes); // On cr�e un contexte OpenGL 3 gr�ce aux attributs donn�s
		wglMakeCurrent(NULL, NULL); // On supprime le contexte temporaire
		wglDeleteContext(tempOpenGLContext); // On supprime le contexte OpenGL 2.1 temporaire
		wglMakeCurrent(hdc, hrc); // On passe au contexte OpenGL 3.0
	}
	else {
		hrc = tempOpenGLContext; // S'il n'y a pas de support d'OpenGL 3.x ou sup�rieur, on utilise le contexte OpenGL 2.1. 
	}

	int glVersion[2] = { -1, -1 }; // Des valeurs par d�faut pour la version
	glGetIntegerv(GL_MAJOR_VERSION, &glVersion[0]); // On r�cup�re la version majeure d'OpenGL utilis�e
	glGetIntegerv(GL_MINOR_VERSION, &glVersion[1]); // On r�cup�re la version mineure d'OpenGL utilis�e

	//std::cout << "Using OpenGL: " << glVersion[0] << "." << glVersion[1] << std::endl;
	// On affiche la version d'OpenGL utilis�e.   

	return true; // On a bien cr�� un contexte, on retourne vrai
}

/**setupScene contient tout ce qui doit �tre initialis� avant rendu.*/
void OpenGLContext::setupScene(Viewer& v) {
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // On d�finit la couleur � la valeur par d�faut de XNA
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	v.InitScene(*this);

	/*createSquare();
	createTexturedSquare();
	createGrid();
	createAxis();*/
	//createTexturedRectangle();
}

/**reshapeWindow est appel�e � chaque fois que la fen�tre est redimensionn�e,
elle d�finit windowWidth et windowHeight, taille du viewport.*/
void OpenGLContext::reshapeWindow(int w, int h) {
	windowWidth = w; // On d�finit la largeur
	windowHeight = h; // On d�finit la hauteur
}

/**renderScene contient tout le code dessin.

La premi�re chose � faire est de d�finir une zone d'affichage pour remplir
toute la fen�tre.

Ensuite, on vide les tampons COLOR, DEPTH et STENCIL pour �viter que les
affichages se superposent.

Tout le reste du code de rendu ira ici.

Finalement, on �change les tampons.*/
void OpenGLContext::renderScene(Camera& camera, Viewer& v) {
	glViewport(0, 0, windowWidth, windowHeight); // On d�finit la zone de dessin � la fen�tre
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); // On vide les tampons. 

	/*shader->bind(); // Lier le shader
	projectionMatrix = glm::perspective(camera.Zoom, (float)windowWidth / (float)windowHeight, 0.1f, 100.f);  // On cr�e la matrice de projection
	glDisable(GL_DEPTH_TEST);
	projectionMatrix = glm::ortho(-1.0f, 1.0f, 1.0f, -1.0f, -10.0f, 1000.0f);
	glEnable(GL_DEPTH_TEST);
	shader->unbind(); // D�lier le shader. */

	v.RenderScene(*this,camera, windowHeight,windowWidth);

	check_gl_error();
	SwapBuffers(hdc); // On �change les tampons pour afficher quelque chose. 
}

void OpenGLContext::createConvexPolygon(float* trueVertices, int verticesCount, float* trueCouleur, unsigned int& vao, unsigned int* vbo) {
	float* vertices = new float[3*3 * (verticesCount - 2)];//3 Sommets pour 3 valeurs(x,y,z)
	float* couleurs = new float[3*3 * (verticesCount - 2)];
	for (int i = 0; i < (verticesCount - 2); i++) {
		vertices[i * 9] = trueVertices[0];
		vertices[i * 9 + 1] = trueVertices[1];
		vertices[i * 9 + 2] = trueVertices[2];
		vertices[i * 9 + 3] = trueVertices[(i+1) * 3];
		vertices[i * 9 + 4] = trueVertices[(i + 1) * 3 + 1];
		vertices[i * 9 + 5] = trueVertices[(i + 1) * 3 + 2];
		vertices[i * 9 + 6] = trueVertices[(i + 1) * 3 + 3];
		vertices[i * 9 + 7] = trueVertices[(i + 1) * 3 + 4];
		vertices[i * 9 + 8] = trueVertices[(i + 1) * 3 + 5];
	}
	for (int i = 0; i < 3 * (verticesCount - 2); i++) {
		couleurs[i * 3] = trueCouleur[0];
		couleurs[i * 3+1] = trueCouleur[1];
		couleurs[i * 3+2] = trueCouleur[2];
	}
	glGenVertexArrays(1, &vao); // Cr�er le VAO
	glBindVertexArray(vao); // Lier le VAO pour l'utiliser

	glGenBuffers(2, &vbo[0]); // G�n�rer les deux VBO

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]); // Lier le VBO
	glBufferData(GL_ARRAY_BUFFER, 9 * (verticesCount - 2) * sizeof(GLfloat), vertices, GL_STATIC_DRAW); // D�finir la taille, les donn�es et le type du VBO
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0); // D�finir le pointeur d'attributs des sommets
	glEnableVertexAttribArray(0); // D�sactiver le VAO	

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]); // Lier le deuxi�me VBO
	glBufferData(GL_ARRAY_BUFFER, 9 * (verticesCount - 2) * sizeof(GLfloat), couleurs, GL_STATIC_DRAW); // D�finir la taille et les donn�es du VBO et le d�finir � STATIC_DRAW  
	glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, 0); // D�finir le pointeur d'attributs des ar�tes
	glEnableVertexAttribArray(1); // Activer le deuxi�me tableau d'attributs des ar�tes

	glBindVertexArray(0); // D�sactiver le VBO
}

void OpenGLContext::createUniqueQuad(unsigned int& vao, unsigned int& vbo) {
	float vertices[] = { 0.0, 0.0, 0.0,   
						 1.0, 0.0, 0.0,
						 1.0, 0.0, 1.0,
						 0.0, 0.0, 1.0};

	GLuint elements[] = {
		0, 1, 2,
		2, 3, 0
	};



	glGenVertexArrays(1, &vao); // Cr�er le VAO
	glBindVertexArray(vao); // Lier le VAO pour l'utiliser

	glGenBuffers(1, &vbo);

	GLuint elementbuffer;
	glGenBuffers(1, &elementbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), &elements[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo); // Lier le VBO
	glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(GLfloat), vertices, GL_STATIC_DRAW); // D�finir la taille, les donn�es et le type du VBO
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0); // D�finir le pointeur d'attributs des sommets
	glEnableVertexAttribArray(0); // D�sactiver le VAO	


	glBindVertexArray(0); // D�sactiver le VBO
}
/**createSquare est utilis�e pour cr�er le VAO qui contiendra le carr�. On stocke en dur ses sommets.*/
void OpenGLContext::createTexturedRectangle(unsigned int& rectangleTexturedVAO, unsigned int* rectangleTexturedVBO) {
	float vertices[] = { -1.0, 0.0, -1.0,   1.0, 0.0, -1.0,  -1.0, 0.0, 1.0,
						 1.0, 0.0, 1.0,		-1.0, 0.0, 1.0,  1.0, 0.0, -1.0 };
	float UV[] = { 0.0, 1.0-0.0,   1.0, 1.0 - 0.0,  0.0, 1.0 - 1.0,
				   1.0, 1.0 - 1.0,   0.0, 1.0 - 1.0,  1.0, 1.0 - 0.0 };

	glGenVertexArrays(1, &rectangleTexturedVAO); // Cr�er le VAO
	glBindVertexArray(rectangleTexturedVAO); // Lier le VAO pour l'utiliser

	glGenBuffers(2, &rectangleTexturedVBO[0]); // G�n�rer les deux VBO

	glBindBuffer(GL_ARRAY_BUFFER, rectangleTexturedVBO[0]); // Lier le VBO
	glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(GLfloat), vertices, GL_STATIC_DRAW); // D�finir la taille, les donn�es et le type du VBO
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0); // D�finir le pointeur d'attributs des sommets
	glEnableVertexAttribArray(0); // D�sactiver le VAO	

	glBindBuffer(GL_ARRAY_BUFFER, rectangleTexturedVBO[1]); // Lier le deuxi�me VBO
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), UV, GL_STATIC_DRAW); // D�finir la taille et les donn�es du VBO et le d�finir � STATIC_DRAW  
	glVertexAttribPointer((GLuint)1, 2, GL_FLOAT, GL_FALSE, 0, 0); // D�finir le pointeur d'attributs des ar�tes
	glEnableVertexAttribArray(1); // Activer le deuxi�me tableau d'attributs des ar�tes

	glBindVertexArray(0); // D�sactiver le VBO
}
void OpenGLContext::createSquare(void) {
	//float* vertices = new float[18];  // Sommets du carr�
	//float *colors = new float[18]; // Couleurs pour les ar�tes  

	float vertices[] = { -1.0, -1.0, -1.0,   1.0, -1.0, -1.0,   1.0, 1.0, -1.0, // Face 1
						-1.0, -1.0, -1.0,   -1.0, 1.0, -1.0,   1.0, 1.0, -1.0,  // Face 1
						1.0, -1.0, 1.0,   1.0, -1.0, -1.0,   1.0, 1.0, -1.0,    // Face 2
						1.0, -1.0, 1.0,   1.0, 1.0, 1.0,   1.0, 1.0, -1.0,      // Face 2
						-1.0, -1.0, 1.0,   1.0, -1.0, 1.0,   1.0, -1.0, -1.0,   // Face 3
						-1.0, -1.0, 1.0,   -1.0, -1.0, -1.0,   1.0, -1.0, -1.0, // Face 3
						-1.0, -1.0, 1.0, 1.0, -1.0, 1.0, 1.0, 1.0, 1.0,			// Face 4
						-1.0, -1.0, 1.0, -1.0, 1.0, 1.0, 1.0, 1.0, 1.0,         // Face 4
						-1.0, -1.0, -1.0, -1.0, -1.0, 1.0, -1.0, 1.0, 1.0,      // Face 5
						-1.0, -1.0, -1.0, -1.0, 1.0, -1.0, -1.0, 1.0, 1.0,      // Face 5
						-1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, -1.0,          // Face 6
						-1.0, 1.0, 1.0, -1.0, 1.0, -1.0, 1.0, 1.0, -1.0
};      // Face 6


	float couleurs[] = { 1.0, 0.0, 0.0,   1.0, 0.0, 0.0,   1.0, 0.0, 0.0,       // Face 1
						1.0, 0.0, 0.0,   1.0, 0.0, 0.0,   1.0, 0.0, 0.0,        // Face 1
						0.0, 1.0, 0.0,   0.0, 1.0, 0.0,   0.0, 1.0, 0.0,        // Face 2
						0.0, 1.0, 0.0,   0.0, 1.0, 0.0,   0.0, 1.0, 0.0,        // Face 2
						0.0, 0.0, 1.0,   0.0, 0.0, 1.0,   0.0, 0.0, 1.0,        // Face 3
						0.0, 0.0, 1.0,   0.0, 0.0, 1.0,   0.0, 0.0, 1.0,        // Face 3 
						1.0, 1.0, 0.0,   1.0, 1.0, 0.0,   1.0, 1.0, 0.0,        // Face 4
						1.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0, 1.0, 0.0,			// Face 4
						0.0, 1.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0, 1.0,            // Face 5
						0.0, 1.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0, 1.0,            // Face 5
						1.0, 0.0, 1.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0,            // Face 6
						1.0, 0.0, 1.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0};           // Face 6

	glGenVertexArrays(1, &vaoID); // Cr�er le VAO
	glBindVertexArray(vaoID); // Lier le VAO pour l'utiliser

	glGenBuffers(2, &vboID[0]); // G�n�rer les deux VBO

	glBindBuffer(GL_ARRAY_BUFFER, vboID[0]); // Lier le VBO
	glBufferData(GL_ARRAY_BUFFER, 108 * sizeof(GLfloat), vertices, GL_STATIC_DRAW); // D�finir la taille, les donn�es et le type du VBO
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0); // D�finir le pointeur d'attributs des sommets
	glEnableVertexAttribArray(0); // D�sactiver le VAO	
	
	glBindBuffer(GL_ARRAY_BUFFER, vboID[1]); // Lier le deuxi�me VBO
	glBufferData(GL_ARRAY_BUFFER, 108 * sizeof(GLfloat), couleurs, GL_STATIC_DRAW); // D�finir la taille et les donn�es du VBO et le d�finir � STATIC_DRAW  
	glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, 0); // D�finir le pointeur d'attributs des ar�tes
	glEnableVertexAttribArray(1); // Activer le deuxi�me tableau d'attributs des ar�tes

	glBindVertexArray(0); // D�sactiver le VBO

	//delete vertices; // Supprimer les sommets
	//delete couleurs; // Supprimer les sommets
}

/**createSquare est utilis�e pour cr�er le VAO qui contiendra le carr�. On stocke en dur ses sommets.*/
void OpenGLContext::createTexturedSquare(void) {
	//float* vertices = new float[18];  // Sommets du carr�
	//float *colors = new float[18]; // Couleurs pour les ar�tes  

	float vertices[] = { -1.0, -1.0, -1.0,   1.0, -1.0, -1.0,   1.0, 1.0, -1.0, // Face 1
		-1.0, -1.0, -1.0,   -1.0, 1.0, -1.0,   1.0, 1.0, -1.0,  // Face 1
		1.0, -1.0, 1.0,   1.0, -1.0, -1.0,   1.0, 1.0, -1.0,    // Face 2
		1.0, -1.0, 1.0,   1.0, 1.0, 1.0,   1.0, 1.0, -1.0,      // Face 2
		-1.0, -1.0, 1.0,   1.0, -1.0, 1.0,   1.0, -1.0, -1.0,   // Face 3
		-1.0, -1.0, 1.0,   -1.0, -1.0, -1.0,   1.0, -1.0, -1.0, // Face 3
		-1.0, -1.0, 1.0, 1.0, -1.0, 1.0, 1.0, 1.0, 1.0,			// Face 4
		-1.0, -1.0, 1.0, -1.0, 1.0, 1.0, 1.0, 1.0, 1.0,         // Face 4
		-1.0, -1.0, -1.0, -1.0, -1.0, 1.0, -1.0, 1.0, 1.0,      // Face 5
		-1.0, -1.0, -1.0, -1.0, 1.0, -1.0, -1.0, 1.0, 1.0,      // Face 5
		-1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, -1.0,          // Face 6
		-1.0, 1.0, 1.0, -1.0, 1.0, -1.0, 1.0, 1.0, -1.0
	};      // Face 6


	float couleurs[] = {0.0, 0.0,   1.0, 0.0,   1.0, 1.0,       // Face 1
		0.0, 0.0,   0.0, 1.0,   1.0, 1.0,					
		0.0, 0.0,   1.0, 0.0,   1.0, 1.0,       // Face 2
		0.0, 0.0,   0.0, 1.0,   1.0, 1.0,
		0.0, 0.0,   1.0, 0.0,   1.0, 1.0,       // Face 3
		0.0, 0.0,   0.0, 1.0,   1.0, 1.0,
		0.0, 0.0,   1.0, 0.0,   1.0, 1.0,       // Face 4
		0.0, 0.0,   0.0, 1.0,   1.0, 1.0,
		0.0, 0.0,   1.0, 0.0,   1.0, 1.0,       // Face 5
		0.0, 0.0,   0.0, 1.0,   1.0, 1.0,
		0.0, 0.0,   1.0, 0.0,   1.0, 1.0,       // Face 6
		0.0, 0.0,   0.0, 1.0,   1.0, 1.0 };       

	glGenVertexArrays(1, &cubetexturedVAO); // Cr�er le VAO
	glBindVertexArray(cubetexturedVAO); // Lier le VAO pour l'utiliser

	glGenBuffers(2, &cubetexturedVBO[0]); // G�n�rer les deux VBO

	glBindBuffer(GL_ARRAY_BUFFER, cubetexturedVBO[0]); // Lier le VBO
	glBufferData(GL_ARRAY_BUFFER, 108 * sizeof(GLfloat), vertices, GL_STATIC_DRAW); // D�finir la taille, les donn�es et le type du VBO
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0); // D�finir le pointeur d'attributs des sommets
	glEnableVertexAttribArray(0); // D�sactiver le VAO	

	glBindBuffer(GL_ARRAY_BUFFER, cubetexturedVBO[1]); // Lier le deuxi�me VBO
	glBufferData(GL_ARRAY_BUFFER, 72 * sizeof(GLfloat), couleurs, GL_STATIC_DRAW); // D�finir la taille et les donn�es du VBO et le d�finir � STATIC_DRAW  
	glVertexAttribPointer((GLuint)1, 2, GL_FLOAT, GL_FALSE, 0, 0); // D�finir le pointeur d'attributs des ar�tes
	glEnableVertexAttribArray(1); // Activer le deuxi�me tableau d'attributs des ar�tes

	glBindVertexArray(0); // D�sactiver le VBO

						  //delete vertices; // Supprimer les sommets
						  //delete couleurs; // Supprimer les sommets
}

void OpenGLContext::createAxis(void) {
	float vertices[] = { 0.0, 0.0, 0.0,	  1.0, 0.0, 0.0,   0.0, 0.0, 0.0,  0.0, 1.0, 0.0,   0.0, 0.0, 0.0,   0.0, 0.0, 1.0 };
	float couleurs[] = { 1.0, 0.0, 0.0,   1.0, 0.0, 0.0,   0.0, 1.0, 0.0,  0.0, 1.0, 0.0,   0.0, 0.0, 1.0,   0.0, 0.0, 1.0 };
	glGenVertexArrays(1, &axisVAO); // Cr�er le VAO
	glBindVertexArray(axisVAO); // Lier le VAO pour l'utiliser

	glGenBuffers(2, &axisVBO[0]); // G�n�rer le VBO

	glBindBuffer(GL_ARRAY_BUFFER, axisVBO[0]); // Lier le VBO
	glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(GLfloat), vertices, GL_STATIC_DRAW); // D�finir la taille, les donn�es et le type du VBO
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0); // D�finir le pointeur d'attributs des sommets
	glEnableVertexAttribArray(0); // D�sactiver le VAO	

	glBindBuffer(GL_ARRAY_BUFFER, axisVBO[1]); // Lier le deuxi�me VBO
	glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(GLfloat), couleurs, GL_STATIC_DRAW); // D�finir la taille et les donn�es du VBO et le d�finir � STATIC_DRAW  
	glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, 0); // D�finir le pointeur d'attributs des ar�tes
	glEnableVertexAttribArray(1); // Activer le deuxi�me tableau d'attributs des ar�tes

	glBindVertexArray(0); // D�sactiver le VBO

}

void OpenGLContext::createGrid(void) {
	float* vertices = new float[252];  // Sommets du carr�
	int j = 0;
	for (GLfloat i = -2.5; i <= 2.5; i += 0.25) {
		vertices[0 + j * 12] = i; vertices[1 + j * 12] = 0; vertices[2 + j * 12] = 2.5; // Coin en bas � gauche
		vertices[3 + j * 12] = i; vertices[4 + j * 12] = 0; vertices[5 + j * 12] = -2.5; // Coin en bas � gauche
		vertices[6 + j * 12] = 2.5; vertices[7 + j * 12] = 0; vertices[8 + j * 12] = i; // Coin en bas � gauche
		vertices[9 + j * 12] = -2.5; vertices[10 + j * 12] = 0; vertices[11 + j * 12] = i; // Coin en bas � gauche
		++j;
	}

	glGenVertexArrays(1, &gridVAO); // Cr�er le VAO
	glBindVertexArray(gridVAO); // Lier le VAO pour l'utiliser

	glGenBuffers(1, &gridVBO); // G�n�rer le VBO

	glBindBuffer(GL_ARRAY_BUFFER, gridVBO); // Lier le VBO
	glBufferData(GL_ARRAY_BUFFER, 252 * sizeof(GLfloat), vertices, GL_STATIC_DRAW); // D�finir la taille, les donn�es et le type du VBO
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0); // D�finir le pointeur d'attributs des sommets
	glEnableVertexAttribArray(0); // D�sactiver le VAO	

	glBindVertexArray(0); // D�sactiver le VBO

	delete[] vertices; // Supprimer les sommets
}

