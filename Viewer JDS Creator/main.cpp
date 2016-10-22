#include "main.h"  
#include "opengl_3.h"  
#include "Camera.h"  
#include "Viewer.h"  
#include "Windowsx.h"


OpenGLContext openglContext; // La classe de contexte

bool running = true; // L'application est-elle lancée ? 
Camera camera;
Viewer v;
HINSTANCE hInstance; // La HINSTANCE de cette application  
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM); // Méthode de rappel standard

bool createWindow(LPCWSTR title, int width, int height) {
	WNDCLASS windowClass;
	HWND hWnd;
	DWORD dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;

	hInstance = GetModuleHandle(NULL);

	windowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	windowClass.lpfnWndProc = (WNDPROC)WndProc;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = hInstance;
	windowClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	windowClass.hbrBackground = NULL;
	windowClass.lpszMenuName = NULL;
	windowClass.lpszClassName = title;

	if (!RegisterClass(&windowClass)) {
		return false;
	}

	hWnd = CreateWindowEx(dwExStyle, title, title, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, width, height, NULL, NULL, hInstance, NULL);

	openglContext.create30Context(hWnd); // On crée le contexte OpenGL dans la fenêtre juste créée

	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

	return true;
}
/**
Gere les appels claviers et souris
*/

void Keyboard_Input()
{
	if (GetKeyState(VK_UP) & 0x80)
	{
		int a = 0;
	}

	if (GetKeyState(VK_DOWN) & 0x80)
	{
		int a = 0;
	}
}

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR    lpCmdLine,
	int       nCmdShow) {
	MSG msg;

	/**
	Les six lignes suivantes effectuent la conversion entre les tableaux de caractères
	et les variables LPCWSTR utilisées par l'API Windows.
	*/
	char *orig = "OpenGL 3 Project"; // Le titre de la fenêtre
	size_t origsize = strlen(orig) + 1;
	const size_t newsize = 100;
	size_t convertedChars = 0;
	wchar_t wcstring[newsize];
	mbstowcs_s(&convertedChars, wcstring, origsize, orig, _TRUNCATE);
	createWindow(wcstring, 500, 500); // On crée la fenêtre OpenGL
	openglContext.setupScene(v); // On prépare la scène OpenGL
	
	while (running)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) { // S'il y a un message à gérer, on s'en occupe
			if (msg.message == WM_QUIT) {
				running = false; // Si on doit quitter, on met running à la valeur faux
			}
			else {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		openglContext.renderScene(camera,v); // On affiche la scène (qui s'occupe aussi d'échanger les tampons)
	}
	return (int)msg.wParam;
}
/** WndProc est une méthode standard utilisée en programmation Win32 pour la gestion des messages à la fenêtre. 
Ici, on gère le redimensionnement de la fenêtre en donnant au contexte la nouvelle taille. */  
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	int a; 
	switch (message) {
	case WM_SIZE: // En cas de redimensionnement
		openglContext.reshapeWindow(LOWORD(lParam), HIWORD(lParam)); // Envoyer la nouvelle taille au contexte
		break;
	case WM_DESTROY: // En cas de fermeture
		PostQuitMessage(0);
		break;
	case WM_MOUSEWHEEL:
		camera.ProcessMouseScroll(GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA);
		break;
	case WM_LBUTTONDOWN: {
		int xPos = GET_X_LPARAM(lParam);
		int yPos = GET_Y_LPARAM(lParam);
		v.xMousePick = xPos;
		v.yMousePick = yPos;
		break;
	}
	case WM_MOUSEMOVE: {
		int xPos = GET_X_LPARAM(lParam);
		int yPos = GET_Y_LPARAM(lParam);
		if (wParam & MK_CONTROL && wParam & MK_LBUTTON)
			camera.ProcessMouseRotation(xPos, yPos);
		camera.SetLastX(xPos);
		break; 
	}
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_UP:
			// Process the UP ARROW key. 
			a = 0;
			camera.ProcessKeyboard(FORWARD,1);
			break;
		case VK_DOWN:
			// Process the DOWN ARROW key.  
			a = 0;
			camera.ProcessKeyboard(BACKWARD,1);
			break; 
		case VK_RIGHT:
			// Process the UP ARROW key. 
			a = 0;
			camera.ProcessKeyboard(RIGHT, 1);
			break;
		case VK_LEFT:
			// Process the DOWN ARROW key.  
			a = 0;
			camera.ProcessKeyboard(LEFT, 1);
			break;
		case 65: //a
			v.bViewSelectionZone = !v.bViewSelectionZone;
			break;
		case WM_IME_NOTIFY:
			a = 0;
			break;
		}
	}
	return DefWindowProc(hWnd, message, wParam, lParam);
}