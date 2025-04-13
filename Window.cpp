#include "Window.h"

Window::Window()
{
	width = 800;
	height = 600;
	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}
Window::Window(GLint windowWidth, GLint windowHeight)
{
	width = windowWidth;
	height = windowHeight;
	muevex = 2.0f;

	avanzarCarro = 0;
	retrocederCarro = 0;
	capo = 0;
	bandera1 = 0;

	avanzarHeli = 0;
	retrocederHeli = 0;

	lucesExtra = 0;

	lucesCarro = -1;

	//pastel = 0;

	for (size_t i = 0; i < 1024; i++)
	{
		keys[i] = 0;
	}
}
int Window::Initialise()
{
	//Inicialización de GLFW
	if (!glfwInit())
	{
		printf("Falló inicializar GLFW");
		glfwTerminate();
		return 1;
	}
	//Asignando variables de GLFW y propiedades de ventana
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//para solo usar el core profile de OpenGL y no tener retrocompatibilidad
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//CREAR VENTANA
	mainWindow = glfwCreateWindow(width, height, "Practica 08. Iluminacion 2", NULL, NULL);

	if (!mainWindow)
	{
		printf("Fallo en crearse la ventana con GLFW");
		glfwTerminate();
		return 1;
	}
	//Obtener tamaño de Buffer
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	//asignar el contexto
	glfwMakeContextCurrent(mainWindow);

	//MANEJAR TECLADO y MOUSE
	createCallbacks();


	//permitir nuevas extensiones
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("Falló inicialización de GLEW");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST); //HABILITAR BUFFER DE PROFUNDIDAD
							 // Asignar valores de la ventana y coordenadas
							 
							 //Asignar Viewport
	glViewport(0, 0, bufferWidth, bufferHeight);
	//Callback para detectar que se está usando la ventana
	glfwSetWindowUserPointer(mainWindow, this);
}

void Window::createCallbacks()
{
	glfwSetKeyCallback(mainWindow, ManejaTeclado);
	glfwSetCursorPosCallback(mainWindow, ManejaMouse);
}
GLfloat Window::getXChange()
{
	GLfloat theChange = xChange;
	xChange = 0.0f;
	return theChange;
}

GLfloat Window::getYChange()
{
	GLfloat theChange = yChange;
	yChange = 0.0f;
	return theChange;
}

void Window::ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}


	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			theWindow->keys[key] = true;
			//printf("se presiono la tecla %d'\n", key);
		}
		else if (action == GLFW_RELEASE)
		{
			theWindow->keys[key] = false;
			//printf("se solto la tecla %d'\n", key);
		}
	}

	if (key == GLFW_KEY_U)
	{
		theWindow->avanzarCarro += 0.1;
	}

	if (key == GLFW_KEY_Y)
	{
		theWindow->retrocederCarro -= 0.1;
	}

	if (key == GLFW_KEY_F)
	{
		if (theWindow->capo < 45.0 && theWindow->bandera1 == 0) {
			theWindow->capo += 2.0;
		}
		else if (theWindow->capo >= 45.0) {
			theWindow->bandera1 = 1;
			theWindow->capo -= 2.0;
		}
		else if (theWindow->capo > -2.0 && theWindow->bandera1 == 1) {
			theWindow->capo -= 2.0;
		}
		else {
			theWindow->bandera1 = 0;
			theWindow->capo += 2.0;
		}
	}

	if (key == GLFW_KEY_J)
	{
		theWindow->avanzarHeli -= 10.0;
	}

	if (key == GLFW_KEY_K)
	{
		theWindow->retrocederHeli += 10.0;
	}

	if (key == GLFW_KEY_M && action == GLFW_PRESS) {
		if (theWindow->lucesExtra == 1) {
			theWindow->lucesExtra = 0;
		}
		else {
			theWindow->lucesExtra = 1;
		}
	}

	if (key == GLFW_KEY_F) {
		theWindow->lucesCarro = 0;
	}
	else if (key == GLFW_KEY_Y) {
		theWindow->lucesCarro = 1;
	}
	else if (key == GLFW_KEY_U) {
		theWindow->lucesCarro = 2;
	}

	if (key == GLFW_KEY_N && action == GLFW_PRESS) {
		if (theWindow->Tele == 1) {
			theWindow->Tele = 0;
		}
		else {
			theWindow->Tele = 1;
		}
	}
}


void Window::ManejaMouse(GLFWwindow* window, double xPos, double yPos)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (theWindow->mouseFirstMoved)
	{
		theWindow->lastX = xPos;
		theWindow->lastY = yPos;
		theWindow->mouseFirstMoved = false;
	}

	theWindow->xChange = xPos - theWindow->lastX;
	theWindow->yChange = theWindow->lastY - yPos;

	theWindow->lastX = xPos;
	theWindow->lastY = yPos;
}


Window::~Window()
{
	glfwDestroyWindow(mainWindow);
	glfwTerminate();

}
