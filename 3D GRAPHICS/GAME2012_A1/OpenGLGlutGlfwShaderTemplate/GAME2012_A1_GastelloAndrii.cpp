//***************************************************************************
// GAME2012 - Assignment1 by Gastello Andrii - 101324434
//
// Assignment 1 submission.
//
// Description:
// Click run to see the results.
//
//*****************************************************************************

#include <iostream>
using namespace std;

#include <stdlib.h>
#include <ctime>
#include <GL/glew.h>
#include <GL/freeglut.h> 
#include "prepShader.h"
#include "glm.hpp"
#include "gtc\matrix_transform.hpp"
#include <string>
#include <array>

#define BUFFER_OFFSET(x)  ((const void*) (x))
#define FPS 30
#define X_AXIS glm::vec3(1,0,0)
#define Y_AXIS glm::vec3(0,1,0)
#define Z_AXIS glm::vec3(0,0,1)

static unsigned int
vertexShaderId,
fragmentShaderId;

enum Buffer_IDs { ArrayBuffer, NumBuffers };

GLuint Buffers[NumBuffers];

const GLuint NumVertices = 20;
//const GLfloat scale = 0.5f;
GLfloat vertices[NumVertices][2];
std::array<glm::vec2, NumVertices> vertices1;

int numOf_square = 15;

GLuint vao, points_vbo, colors_vbo, modelID, program,
model;

glm::mat4 Model;
glm::mat4 myIdentityMatrix;
glm::mat4 ScaleMatrix;
glm::mat4 RotationMatrix;

float rotAngle;
int numbers;
float* colours;
float** colourIndex;

GLfloat points[] = {
	-0.9f,  0.9f,  0.0f,
	0.9f,  0.9f,  0.0f,
	0.9f, -0.9f,  0.0f,
	-0.9f, -0.9f,  0.0f
};

GLfloat colors[] = {
	00.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	1.0f, 0.0f, 1.0f
};

float scale = 1.0f, inc = -0.05f, angle = 0.0f;

void timer(int); // Prototype.

void init(void)
{
	// Create shader program executable.
	vertexShaderId = setShader((char*)"vertex", (char*)"transform.vert");
	fragmentShaderId = setShader((char*)"fragment", (char*)"cube.frag");
	program = glCreateProgram();
	glAttachShader(program, vertexShaderId);
	glAttachShader(program, fragmentShaderId);
	glLinkProgram(program);
	glUseProgram(program);

	modelID = glGetUniformLocation(program, "model");

	vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

		points_vbo = 0;
		glGenBuffers(1, &points_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
			glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), points, GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
			glEnableVertexAttribArray(0);

		colors_vbo = 0;
		glGenBuffers(1, &colors_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
			glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), colors, GL_STATIC_DRAW);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
			glEnableVertexAttribArray(1);
		
		glBindBuffer(GL_ARRAY_BUFFER, 0); // Can optionally unbind the buffer to avoid modification.

	glBindVertexArray(0); // Can optionally unbind the vertex array to avoid modification.

	timer(0);
	glClearColor(0.4, 0.4, 0.4, 1.0); 
}

//---------------------------------------------------------------------
//
// transformModel
//
void transformObject(float scale, glm::vec3 rotationAxis, float rotationAngle, glm::vec3 translation) {
	glm::mat4 Model;
	Model = glm::mat4(1.0f);
	Model = glm::translate(Model, translation);
	Model = glm::rotate(Model, glm::radians(rotationAngle), rotationAxis);
	Model = glm::scale(Model, glm::vec3(scale));
	glUniformMatrix4fv(modelID, 1, GL_FALSE, &Model[0][0]);
}

void recolorSquare(void)
{
	float r = (float)rand() / (float)RAND_MAX;
	float g = (float)rand() / (float)RAND_MAX;
	float b = (float)rand() / (float)RAND_MAX;

	float colours[] = {
		r, g, b,
		r, g, b,
		r, g, b,
		r, g, b
	};
	glBufferData(GL_ARRAY_BUFFER, sizeof(colours), colours, GL_STATIC_DRAW); // Why can we do this without binding again?
}

//---------------------------------------------------------------------
//
// display
//

void display(void)
{
	float x = 19.5f,rot=0.0f;
	glClear(GL_COLOR_BUFFER_BIT);
	int i = 0;
	
	do
	{
		glBindVertexArray(i);
		//Transform each of the squares correctly (here only need to scale and rotate them).
		x = (x / (1+glm::tan(glm::radians(angle)))/glm::cos(glm::radians(angle)))  ;
		//transformObject(pow(scale, i), Z_AXIS, angle * i, glm::vec3(0.0f, 0.0f, 0.0f));
		transformObject(x, Z_AXIS, rot-=angle, glm::vec3(0.0f, 0.0f, 0.0f));
		//Apply a different color for each of the squares.
		recolorSquare();

		glDrawArrays(GL_LINE_LOOP, 0, 4);
		i++;
	} while (i < numOf_square+1);

	//glBindVertexArray(0); // Can optionally unbind the vertex array to avoid modification.

	//glutSwapBuffers();
	glFlush();
}


void timer(int) {
	glutPostRedisplay();
	glutTimerFunc(1000 / FPS, timer, 0);
}

void idle() // Not even called.
{
	glutPostRedisplay();
}
// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:
		exit(0);
		break;
	default:
		break;
	}
}

//---------------------------------------------------------------------
//
// main
//

int main(int argc, char** argv)
{
	//Before we can open a window, theremust be interaction between the windowing systemand OpenGL.In GLUT, this interaction is initiated by the following function call :
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);

	cout << "How many squares do you wish ? : "; cin >> numOf_square;
	cout << "\nWhat the angle ? : ";cin >> angle;
	
	//if you comment out this line, a window is created with a default size
	glutInitWindowSize(768, 768);

	//the top-left corner of the display
	glutInitWindowPosition(124, 124);

	glutCreateWindow("Gastello Andrii - 101324434");

	glewInit();	//Initializes the glew and prepares the drawing pipeline.

	init(); 

	
	glutDisplayFunc(display);

	
	glutKeyboardFunc(keyInput);
	
	glutMainLoop();
}