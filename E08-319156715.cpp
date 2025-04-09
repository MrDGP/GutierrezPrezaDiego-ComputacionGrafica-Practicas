/*
Práctica 8: Iluminación 2
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;
bool lamparaEncendida = true;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture AgaveTexture;
Texture octaDadoTexture;

Model Kitt_M;
Model Llanta_M;
Model Blackhawk_M;


Model AutoPartes;
Model llantaDerechaDelantera;
Model llantaIzquierdaDelantera;
Model llantaDerechaTrasera;
Model llantaIzquierdaTrasera;
Model Capo;
Model Lampara;

Skybox skybox;

//materiales
Material Material_brillante;
Material Material_opaco;


//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

PointLight pointLightsOff[1];

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";


//función de calculo de normales por promedio de vértices 
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}


void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	unsigned int vegetacionIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	   4,5,6,
	   4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,
	};
	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh *obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);

}


void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}
void CrearOctaedro() {
	unsigned int  octaedroIndices[] = {
		0, 3, 17, //Dado 2
		7, 2, 18, //Dado 3
		6, 1, 19, //Dado 4
		5, 4, 16, //Dado 1

		12, 13, 23, //Dado 8
		8, 11, 20, //Dado 5
		10, 15, 21, //Dado 6
		9, 14, 22, //Dado 7

	};

	GLfloat octaedroVertices[] = {
		0.5f,-0.5f,0.5f,	0.0029f,  0.3984f,		0.0f,	-1.0f,	-1.0f,		//A	0	 2
		0.5f,-0.5f,-0.5f,	0.6611f,  0.0049f,		0.0f,	-1.0f,	1.0f,		//B	1	 4
		-0.5f,-0.5f,-0.5f,	0.6611f,  0.3984f,		1.0f,	-1.0f,	0.0f,		//C	2	 3
		-0.5f,-0.5f,0.5f,	0.3330f,  0.5957f,		0.0f,	-1.0f,	-1.0f,		//D	3	 2

		0.5f,-0.5f,0.5f,	0.0029f,  0.3984f,		-1.0f,	-1.0f,	-1.0f,		//A	4	 1
		0.5f,-0.5f,-0.5f,	0.0039f,  0.0049f,		-1.0f,	-1.0f,	-1.0f,		//B	5	 1
		-0.5f,-0.5f,-0.5f,	0.6611f,  0.3984f,		0.0f,	-1.0f,	1.0f,		//C	6	 4
		-0.5f,-0.5f,0.5f,	0.3330f,  0.5957f,		1.0f,	-1.0f,	0.0f,		//D	7	 3

		0.5f,-0.5f,0.5f,	0.3369f,  0.9902f,		0.0f,	1.0f,	-1.0f,		//A	8	 5
		0.5f,-0.5f,-0.5f,	0.9922f,  0.5986f,		0.0f,	1.0f,	1.0f,		//B	9	 7
		-0.5f,-0.5f,-0.5f,	0.6641f,  0.4043f,		1.0f,	1.0f,	0.0f,		//C	10	 6
		-0.5f,-0.5f,0.5f,	0.3369f,  0.6094f,		0.0f,	1.0f,	-1.0f,		//D	11	 5

		0.5f,-0.5f,0.5f,	0.9961f,  0.6094f,		-1.0f,	1.0f,	-1.0f,		//A	12	 8
		0.5f,-0.5f,-0.5f,	0.9961f,  0.9932f,		-1.0f,	1.0f,	-1.0f,		//B	13	 8
		-0.5f,-0.5f,-0.5f,	0.6689f,  0.4043f,		0.0f,	1.0f,	1.0f,		//C	14	 7
		-0.5f,-0.5f,0.5f,	0.3418f,  0.5996f,		1.0f,	1.0f,	0.0f,		//D	15	 6

		0.0f,0.2f,0.0f,		0.3350f,  0.2002f,		-1.0f,	-1.0f,	-1.0f,		//E	16   1
		0.0f,0.2f,0.0f,		0.3350f,  0.2002f,		0.0f,	-1.0f,	-1.0f,		//E	17   2
		0.0f,0.2f,0.0f,		0.3350f,  0.2002f,		1.0f,	-1.0f,	0.0f,		//E	18   3
		0.0f,0.2f,0.0f,		0.3350f,  0.2002f,		0.0f,	-1.0f,	1.0f,		//E	19   4

		0.0f,-1.2f,0.0f,	0.6641f,  0.7949f,		0.0f,	1.0f,	-1.0f,		//F	20   5
		0.0f,-1.2f,0.0f,	0.6641f,  0.7949f,		1.0f,	1.0f,	0.0f,		//F	21   6
		0.0f,-1.2f,0.0f,	0.6641f,  0.7949f,		0.0f,	1.0f,	1.0f,		//F	22   7
		0.0f,-1.2f,0.0f,	0.6641f,  0.7949f,		-1.0f,	1.0f,	-1.0f,		//F	23   8

	};
	Mesh* octaedro = new Mesh();
	octaedro->CreateMesh(octaedroVertices, octaedroIndices, 192, 24);
	meshList.push_back(octaedro);
}
int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CrearOctaedro();
	CreateShaders();
	


	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	AgaveTexture = Texture("Textures/Agave.tga");
	AgaveTexture.LoadTextureA();
	octaDadoTexture = Texture("Textures/octaDadoA.jpg");
	octaDadoTexture.LoadTexture();

	Kitt_M = Model();
	Kitt_M.LoadModel("Models/kitt_optimizado.obj");
	Llanta_M = Model();
	Llanta_M.LoadModel("Models/llanta_optimizada.obj");
	Blackhawk_M = Model();
	Blackhawk_M.LoadModel("Models/uh60.obj");






	//COCHE
	// auto sin llantas ni capo
	AutoPartes = Model();
	AutoPartes.LoadModel("Models/AutoPartes.obj");

	// llanta derecha delantera
	llantaDerechaDelantera = Model();
	llantaDerechaDelantera.LoadModel("Models/llantaDerechaDelantera.obj");

	//llanta izquierda delantera
	llantaIzquierdaDelantera = Model();
	llantaIzquierdaDelantera.LoadModel("Models/llantaIzquierdaDelantera.obj");

	//llanta derecha trasera
	llantaDerechaTrasera = Model();
	llantaDerechaTrasera.LoadModel("Models/llantaDerechaTrasera.obj");

	//llanta izquierda trasera
	llantaIzquierdaTrasera = Model();
	llantaIzquierdaTrasera.LoadModel("Models/llantaIzquierdaTrasera.obj");

	//capo
	Capo = Model();
	Capo.LoadModel("Models/Capo.obj");
	
	//lampara
	Lampara = Model();
	Lampara.LoadModel("Models/Lampara.obj");

	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);


	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.06f, 0.06f,
		0.0f, -1.0f, 0.0f);									 //para ciclo de dia y noche es con ciclo for pero como ??
	//contador de luces puntuales
	unsigned int pointLightCount = 0;



	//Declaración de primer luz puntual
	pointLights[0] = PointLight(1.0f, 1.0f, 1.0f,//color
		1.0f, 1.0f,				//brillo, difusa
		0.0f, 6.3f, 6.0f,		//posicion
		1.0f, 0.09f, 0.032f);		//coeficientes
	pointLightCount++;

	unsigned int spotLightCount = 0;
	
	//linterna
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f);
	spotLightCount++;
	
	//luz fija
	spotLights[1] = SpotLight(1.0f, 1.0f, 0.0f,
		0.6f, 0.4f,			// (ILUMINA,DIFUSA)
		-2.5f,1.0f, -1.5f,  //POSICION
		-1.0f, -0.0f, 0.0f,  // A DONDE APUNTA 
		1.0f, 0.0f, 0.0f,	//COEFICIENTES
		40.0f);  //RANGO DEL CONO DE ILUMINACION 
	spotLightCount++;
	
	//se crean mas luces puntuales y spotlight 
	
	

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();
		
		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la cámara de tipo flash
		//sirve para que en tiempo de ejecución (dentro del while) se cambien propiedades de la luz
		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;  //LINEA PARA QUE NO QUEDE COMPLETAMENTE AL CENTRO Y NOS CIEGUE 
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection()); // LINEA QYE HACE QUE LA LAMP SE MUEVA CON RESPECTO A LAL CAMARA

		/*
		// Luz que sigue al HELICOPTER HELICOPTER
		glm::vec3 carPos = glm::vec3(0.0f + mainWindow.getmuevex()*2.0f, 5.0f, 6.0f); // posición del coche
		glm::vec3 lightDir = glm::vec3(0.0f, -1.0f, 0.0f); // dirección de la luz 
		spotLights[1].SetFlash(carPos, lightDir);
		*/

		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
		//shaderList[0].SetPointLights(pointLights, pointLightCount);
		if (mainWindow.getLamparaEncendida() == true)
		{
			shaderList[0].SetPointLights(pointLights, pointLightCount);
		}
		else
		{
			shaderList[0].SetPointLights(pointLightsOff, pointLightCount - 1);
		}
		
		shaderList[0].SetSpotLights(spotLights, spotLightCount-1);



		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[2]->RenderMesh();
		
	

		//Dado 
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(2.0f, 5.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		octaDadoTexture.UseTexture();
		meshList[4]->RenderMesh();
		
		
		/*
		//helicopter helicopter
		//Presionar Y, U para moverlo
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 5.0f, 6.0));
		model = glm::translate(model, glm::vec3(mainWindow.getmuevex(), 0.0f, 0.0f));
		model = glm::translate(model, glm::vec3(mainWindow.getmuevex(), 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Blackhawk_M.RenderModel();
		*/

		//lampara
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 2.0f, 6.0));
		model = glm::scale(model, glm::vec3(0.05f, 0.05f, 0.05f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Lampara.RenderModel();

		//Agave ¿qué sucede si lo renderizan antes del coche y el helicóptero?
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, -4.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		
		//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		AgaveTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[3]->RenderMesh();
		glDisable(GL_BLEND);

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
