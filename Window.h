#pragma once
#include<stdio.h>
#include<glew.h>
#include<glfw3.h>

class Window
{
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);
	int Initialise();
	GLfloat getBufferWidth() { return bufferWidth; }
	GLfloat getBufferHeight() { return bufferHeight; }
	bool getShouldClose() {
		return  glfwWindowShouldClose(mainWindow);}
	bool* getsKeys() { return keys; }
	GLfloat getXChange();
	GLfloat getYChange();
	void swapBuffers() { return glfwSwapBuffers(mainWindow); }
	GLfloat getrotay() { return rotay; }
	GLfloat getrotax() { return rotax; }
	GLfloat getrotaz() { return rotaz; }

	//PARA GATO 3D
	GLfloat getarticulacion1() { return articulacion1; }
	GLfloat getarticulacion2() { return articulacion2; }
	GLfloat getarticulacion3() { return articulacion3; }
	GLfloat getarticulacion4() { return articulacion4; }
	GLfloat getarticulacion5() { return articulacion5; }
	GLfloat getarticulacion6() { return articulacion6; }
	GLfloat getarticulacion7() { return articulacion7; }
	GLfloat getarticulacion8() { return articulacion8; }

	//PARA RUEDAS DE GRUA Y ORJEAS Y COLA DE GATO 3D
	GLfloat getcompleta1() { return completa1; }
	GLfloat getcompleta2() { return completa2; }
	GLfloat getcompleta3() { return completa3; }
	GLfloat getcompleta4() { return completa4; }

	//PARA ARTICULACIONES GRUA 
	GLfloat getarticulacionG1() { return articulacionG1; }
	GLfloat getarticulacionG2() { return articulacionG2; }
	GLfloat getarticulacionG3() { return articulacionG3; }
	GLfloat getarticulacionG4() { return articulacionG4; }

	~Window();
private: 
	GLFWwindow *mainWindow;
	GLint width, height;
	GLfloat rotax,rotay,rotaz, articulacion1, articulacion2, articulacion3, articulacion4, articulacion5, articulacion6, articulacion7, articulacion8,completa1,completa2,completa3,completa4;
	GLint bandera1,bandera2,bandera3,bandera4,bandera5,bandera6,bandera7,bandera8;
	GLfloat  articulacionG1, articulacionG2, articulacionG3, articulacionG4;
	
	bool keys[1024];
	GLint bufferWidth, bufferHeight;
	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;
	bool mouseFirstMoved;
	void createCallbacks();
	static void ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode);
	static void ManejaMouse(GLFWwindow* window, double xPos, double yPos);
};

