#include "opengl_3.h"
#include "Viewer.h"

/**Constructeur par défaut de la classe OpenGLContext. À présent, il ne fait rien,
mais on peut y ajouter ce que l'on veut. */
OpenGLContext::OpenGLContext(void) {
}

/**Constructeur de la classe OpenGLContext qui crée un contexte avec un HWND.*/
OpenGLContext::OpenGLContext(HWND hwnd) {
	create30Context(hwnd);
}

/**Destructeur de la classe OpenGLContext, qui va nettoyer le contexte de rendu et
libérer le contexte matériel de la fenêtre courante.*/
OpenGLContext::~OpenGLContext(void) {
	wglMakeCurrent(hdc, 0); // Enlever le contexte de rendu du contexte matériel
	wglDeleteContext(hrc); // Supprimer le contexte de rendu
	ReleaseDC(hwnd, hdc); // Libérer le contexte matériel de la fenêtre
}

/**create30Context crée un contexte OpenGL et l'attache à la fenêtre pointée par le HWND.
Cette méthode crée, pour l'instant, un contexte OpenGL 3.2 par défaut, mais tentera
de créer un contexte OpenGL 2.1 si cela n'est pas possible.*/
bool OpenGLContext::create30Context(HWND hwnd) {
	this->hwnd = hwnd; // On définit le HWND de la fenêtre
	hdc = GetDC(hwnd); // On récupère le contexte matériel pour la fenêtre

	PIXELFORMATDESCRIPTOR pfd; // On crée un nouveau PIXELFORMATDESCRIPTOR (PFD)  
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR)); // que l'on efface
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR); // On en définit la taille à la taille de la structure
	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW; // On active le double buffering, le support d'OpenGL et le dessin sur une fenêtre
	pfd.iPixelType = PFD_TYPE_RGBA; // On utilise des pixels RVBA
	pfd.cColorBits = 32; // Trente-deux bits d'informations de couleur (plus cette valeur est élevée, plus on a de couleurs)
	pfd.cDepthBits = 32; // Trente-deux bits d'information de profondeur (plus cette valeur est élevée, plus on a de niveaux de détails)
	pfd.iLayerType = PFD_MAIN_PLANE; // On définit le calque du PFD

	int nPixelFormat = ChoosePixelFormat(hdc, &pfd); // Vérifie si le PFD est valide et renvoie un format de pixels
	if (nPixelFormat == 0) // Si elle échoue
		return false;

	bool bResult = SetPixelFormat(hdc, nPixelFormat, &pfd); // On essaie de définir le format de pixels
	if (!bResult) // Si elle échoue
		return false;

	HGLRC tempOpenGLContext = wglCreateContext(hdc); // On crée un contexte OpenGL 2.1 pour le contexte matériel
	wglMakeCurrent(hdc, tempOpenGLContext); // On rend ce contexte courant et actif

	glewExperimental = GL_TRUE;
	GLenum error = glewInit(); // On active GLEW  
	if (error != GLEW_OK) // Si cela échoue
		return false;

	int attributes[] = {
		WGL_CONTEXT_MAJOR_VERSION_ARB, 3, // Version majeure d'OpenGL : 3
		WGL_CONTEXT_MINOR_VERSION_ARB, 2, // Version mineure d'OpenGL : 2  
		WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB, // Compatibilité ascendante
		0
	};

	if (wglewIsSupported("WGL_ARB_create_context") == 1) { // Si l'extension de création de contexte OpenGL 3.x est disponible
		hrc = wglCreateContextAttribsARB(hdc, NULL, attributes); // On crée un contexte OpenGL 3 grâce aux attributs donnés
		wglMakeCurrent(NULL, NULL); // On supprime le contexte temporaire
		wglDeleteContext(tempOpenGLContext); // On supprime le contexte OpenGL 2.1 temporaire
		wglMakeCurrent(hdc, hrc); // On passe au contexte OpenGL 3.0
	}
	else {
		hrc = tempOpenGLContext; // S'il n'y a pas de support d'OpenGL 3.x ou supérieur, on utilise le contexte OpenGL 2.1. 
	}

	int glVersion[2] = { -1, -1 }; // Des valeurs par défaut pour la version
	glGetIntegerv(GL_MAJOR_VERSION, &glVersion[0]); // On récupère la version majeure d'OpenGL utilisée
	glGetIntegerv(GL_MINOR_VERSION, &glVersion[1]); // On récupère la version mineure d'OpenGL utilisée

	//std::cout << "Using OpenGL: " << glVersion[0] << "." << glVersion[1] << std::endl;
	// On affiche la version d'OpenGL utilisée.   

	return true; // On a bien créé un contexte, on retourne vrai
}


void OpenGLContext::initAlphabet() {
	OpenGLContext& m = *this;
	// FreeType
	FT_Library ft;
	// All functions return a value different than 0 whenever an error occurred
	if (FT_Init_FreeType(&ft))
		assert(0);
		//std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

	// Load font as face
	FT_Face face;
	if (FT_New_Face(ft, "fonts/arial.ttf", 0, &face))
		assert(0);
		//std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

	// Set size to load glyphs as
	FT_Set_Pixel_Sizes(face, 0, 48);

	// Disable byte-alignment restriction
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);


	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// Load first 128 characters of ASCII set
	for (GLubyte c = 0; c < 128; c++)
	{
		// Load character glyph 
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			assert(0);
			//std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
			continue;
		}
		// Generate texture
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);
		// Set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// Now store character for later use
		Character character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x
		};
		m.Characters.insert(std::pair<GLchar, Character>(c, character));
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	// Destroy FreeType once we're finished
	FT_Done_Face(face);
	FT_Done_FreeType(ft);
}



/**setupScene contient tout ce qui doit être initialisé avant rendu.*/
void OpenGLContext::setupScene() {
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // On définit la couleur à la valeur par défaut de XNA
	glEnable(GL_DEPTH_TEST); 

	//glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthFunc(GL_LESS);
	this->initAlphabet();
}

/**reshapeWindow est appelée à chaque fois que la fenêtre est redimensionnée,
elle définit windowWidth et windowHeight, taille du viewport.*/
void OpenGLContext::reshapeWindow(int w, int h) {
	windowWidth = w; // On définit la largeur
	windowHeight = h; // On définit la hauteur
}

/**renderScene contient tout le code dessin.

La première chose à faire est de définir une zone d'affichage pour remplir
toute la fenêtre.

Ensuite, on vide les tampons COLOR, DEPTH et STENCIL pour éviter que les
affichages se superposent.

Tout le reste du code de rendu ira ici.

Finalement, on échange les tampons.*/
void OpenGLContext::renderScene(Camera& camera, Viewer& v) {
	glViewport(0, 0, windowWidth, windowHeight); // On définit la zone de dessin à la fenêtre
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); // On vide les tampons. 

	/*shader->bind(); // Lier le shader
	projectionMatrix = glm::perspective(camera.Zoom, (float)windowWidth / (float)windowHeight, 0.1f, 100.f);  // On crée la matrice de projection
	glDisable(GL_DEPTH_TEST);
	projectionMatrix = glm::ortho(-1.0f, 1.0f, 1.0f, -1.0f, -10.0f, 1000.0f);
	glEnable(GL_DEPTH_TEST);
	shader->unbind(); // Délier le shader. */

	v.RenderScene(*this,camera, windowHeight,windowWidth);

	check_gl_error();
	SwapBuffers(hdc); // On échange les tampons pour afficher quelque chose. 
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
	glGenVertexArrays(1, &vao); // Créer le VAO
	glBindVertexArray(vao); // Lier le VAO pour l'utiliser

	glGenBuffers(2, &vbo[0]); // Générer les deux VBO

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]); // Lier le VBO
	glBufferData(GL_ARRAY_BUFFER, 9 * (verticesCount - 2) * sizeof(GLfloat), vertices, GL_STATIC_DRAW); // Définir la taille, les données et le type du VBO
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0); // Définir le pointeur d'attributs des sommets
	glEnableVertexAttribArray(0); // Désactiver le VAO	

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]); // Lier le deuxième VBO
	glBufferData(GL_ARRAY_BUFFER, 9 * (verticesCount - 2) * sizeof(GLfloat), couleurs, GL_STATIC_DRAW); // Définir la taille et les données du VBO et le définir à STATIC_DRAW  
	glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, 0); // Définir le pointeur d'attributs des arêtes
	glEnableVertexAttribArray(1); // Activer le deuxième tableau d'attributs des arêtes

	glBindVertexArray(0); // Désactiver le VBO
}

void OpenGLContext::createUniqueQuad3d(unsigned int& vao, unsigned int& vbo) {
	float vertices[] = { 0.0, 0.0, 0.0,
		1.0, 0.0, 0.0,
		1.0, 0.0, 1.0,
		0.0, 0.0, 1.0,
		0.0, -1.0, 0.0,
		1.0, -1.0, 0.0,
		1.0, -1.0, 1.0,
		0.0, -1.0, 1.0 };

	GLuint elements[] = {
		0, 1, 2,    2, 3, 0,
		0, 1, 5,    0, 5, 4,
		1, 2, 6,	1, 6, 5,
		2, 3, 7,	2, 7, 6,
		3, 0, 4,	3, 4, 7,
		4, 5, 6,    6, 7, 4
	};



	glGenVertexArrays(1, &vao); // Créer le VAO
	glBindVertexArray(vao); // Lier le VAO pour l'utiliser

	glGenBuffers(1, &vbo);

	GLuint elementbuffer;
	glGenBuffers(1, &elementbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), &elements[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo); // Lier le VBO
	glBufferData(GL_ARRAY_BUFFER, 108 * sizeof(GLfloat), vertices, GL_STATIC_DRAW); // Définir la taille, les données et le type du VBO
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0); // Définir le pointeur d'attributs des sommets
	glEnableVertexAttribArray(0); // Désactiver le VAO	


	glBindVertexArray(0); // Désactiver le VBO
}
void OpenGLContext::createUniqueQuadTextured(unsigned int& vao, unsigned int* vbo) {
	float vertices[] = { 0.0, 0.0, 0.0,
		1.0, 0.0, 0.0,
		1.0, 1.0, 0.0,
		0.0, 1.0, 0.0 };
	GLuint elements[] = {
		0, 1, 2,
		2, 3, 0
	};
	float UV[] = { 0.0, 1.0 - 0.0,   
				   1.0, 1.0 - 0.0,  
				   1.0, 1.0 - 1.0,
				   0.0, 1.0 - 1.0 };

	glGenVertexArrays(1, &vao); // Créer le VAO
	glBindVertexArray(vao); // Lier le VAO pour l'utiliser

	glGenBuffers(2, &vbo[0]);

	GLuint elementbuffer;
	glGenBuffers(1, &elementbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), &elements[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]); // Lier le VBO
	glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(GLfloat), vertices, GL_STATIC_DRAW); // Définir la taille, les données et le type du VBO
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0); // Définir le pointeur d'attributs des sommets
	glEnableVertexAttribArray(0); // Désactiver le VAO	

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]); // Lier le deuxième VBO
	glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(GLfloat), UV, GL_STATIC_DRAW); // Définir la taille et les données du VBO et le définir à STATIC_DRAW  
	glVertexAttribPointer((GLuint)1, 2, GL_FLOAT, GL_FALSE, 0, 0); // Définir le pointeur d'attributs des arêtes
	glEnableVertexAttribArray(1); // Activer le deuxième tableau d'attributs des arêtes

	glBindVertexArray(0); // Désactiver le VBO
}
void OpenGLContext::createUniqueQuad(unsigned int& vao, unsigned int& vbo) {
	float vertices[] = { 0.0, 0.0, 0.0,   
						 1.0, 0.0, 0.0,
						 1.0, 1.0, 1.0,
						 0.0, 1.0, 1.0};

	GLuint elements[] = {
		0, 1, 2,
		2, 3, 0
	};



	glGenVertexArrays(1, &vao); // Créer le VAO
	glBindVertexArray(vao); // Lier le VAO pour l'utiliser

	glGenBuffers(1, &vbo);

	GLuint elementbuffer;
	glGenBuffers(1, &elementbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), &elements[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo); // Lier le VBO
	glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(GLfloat), vertices, GL_STATIC_DRAW); // Définir la taille, les données et le type du VBO
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0); // Définir le pointeur d'attributs des sommets
	glEnableVertexAttribArray(0); // Désactiver le VAO	


	glBindVertexArray(0); // Désactiver le VBO
}

void OpenGLContext::RenderText(Shader &shader, std::string text, GLfloat x, GLfloat y, GLfloat scale, float* color)
{
	OpenGLContext& m = *this;
    // Activate corresponding render state	
	shader.bind();
	int projectionMatrixLocation = glGetUniformLocation(shader.id(), "projectionMatrix");

	//projectionMatrix = glm::ortho(-1.0f, 1.0f, 1.0f, -1.0f, -10.0f, 1000.0f);
	glm::mat4 projectionMatrix = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f);
	glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
	glUniform3fv(glGetUniformLocation(shader.id(), "Color"),1 , color);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAO);

    // Iterate through all characters
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++) 
    {
        Character ch = m.Characters[*c];

        GLfloat xpos = x + ch.Bearing.x * scale;
        GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        GLfloat w = ch.Size.x * scale;
        GLfloat h = ch.Size.y * scale;
        // Update VBO for each character
        GLfloat vertices[6][4] = {
            { xpos,     ypos + h,   0.0, 0.0 },            
            { xpos,     ypos,       0.0, 1.0 },
            { xpos + w, ypos,       1.0, 1.0 },

            { xpos,     ypos + h,   0.0, 0.0 },
            { xpos + w, ypos,       1.0, 1.0 },
            { xpos + w, ypos + h,   1.0, 0.0 }           
        };
        // Render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // Update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // Render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
	shader.unbind();
}
/**createSquare est utilisée pour créer le VAO qui contiendra le carré. On stocke en dur ses sommets.*/
void OpenGLContext::createTexturedRectangle(unsigned int& rectangleTexturedVAO, unsigned int* rectangleTexturedVBO) {
	float vertices[] = { -1.0, 0.0, -1.0,   1.0, 0.0, -1.0,  -1.0, 0.0, 1.0,
						 1.0, 0.0, 1.0,		-1.0, 0.0, 1.0,  1.0, 0.0, -1.0 };
	float UV[] = { 0.0, 1.0-1.0,   1.0, 1.0 - 1.0,  0.0, 1.0 - 0.0,
				   1.0, 1.0 - 0.0,   0.0, 1.0 - 0.0,  1.0, 1.0 - 1.0 };

	glGenVertexArrays(1, &rectangleTexturedVAO); // Créer le VAO
	glBindVertexArray(rectangleTexturedVAO); // Lier le VAO pour l'utiliser

	glGenBuffers(2, &rectangleTexturedVBO[0]); // Générer les deux VBO

	glBindBuffer(GL_ARRAY_BUFFER, rectangleTexturedVBO[0]); // Lier le VBO
	glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(GLfloat), vertices, GL_STATIC_DRAW); // Définir la taille, les données et le type du VBO
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0); // Définir le pointeur d'attributs des sommets
	glEnableVertexAttribArray(0); // Désactiver le VAO	

	glBindBuffer(GL_ARRAY_BUFFER, rectangleTexturedVBO[1]); // Lier le deuxième VBO
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), UV, GL_STATIC_DRAW); // Définir la taille et les données du VBO et le définir à STATIC_DRAW  
	glVertexAttribPointer((GLuint)1, 2, GL_FLOAT, GL_FALSE, 0, 0); // Définir le pointeur d'attributs des arêtes
	glEnableVertexAttribArray(1); // Activer le deuxième tableau d'attributs des arêtes

	glBindVertexArray(0); // Désactiver le VBO
}