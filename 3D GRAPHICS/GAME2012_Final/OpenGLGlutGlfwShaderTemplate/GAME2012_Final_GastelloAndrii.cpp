//***************************************************************************
// 
// GAME2012_Final_GastelloAndrii.cpp by Gastello Andrii (C) 2021 All Rights Reserved.
// 
// Final Project submission.
//
// Description:
//		Click run to see the results.
//
//*****************************************************************************

using namespace std;

#include "stdlib.h"
#include "time.h"
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "prepShader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <iostream>
#include "Shape.h"
#include "Light.h"

#define BUFFER_OFFSET(x)  ((const void*) (x))
#define FPS 60
#define MOVESPEED 0.1f
#define TURNSPEED 0.05f
#define X_AXIS glm::vec3(1,0,0)
#define Y_AXIS glm::vec3(0,1,0)
#define Z_AXIS glm::vec3(0,0,1)
#define XY_AXIS glm::vec3(1,0.9,0)
#define YZ_AXIS glm::vec3(0,1,1)
#define XZ_AXIS glm::vec3(1,0,1)
#define XYZ_AXIS glm::vec3(1,1,1)
#define SPEED 0.25f

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

enum keyMasks
{
	KEY_FORWARD = 0b00000001,
	// 0x01 or   1	or   01
	KEY_BACKWARD = 0b00000010,
	// 0x02 or   2	or   02
	KEY_LEFT = 0b00000100,
	KEY_RIGHT = 0b00001000,
	KEY_UP = 0b00010000,
	KEY_DOWN = 0b00100000,
	KEY_MOUSECLICKED = 0b01000000
};

static unsigned int
	program,
	vertexShaderId,
	fragmentShaderId;

GLuint modelID, viewID, projID;
glm::mat4 View, Projection;

// Our bitflag variable. 1 byte for up to 8 key states.
unsigned char keys = 0; // Initialized to 0 or 0b00000000.

// Texture vars
GLuint blankID;
GLuint platformID;
GLuint hedgeID;
GLuint wallID;
GLuint wallTopID;
GLuint towerID;
GLuint roofID;

GLint width, height, bitDepth;

GLUquadricObj * quadObj = gluNewQuadric();

// Light Variables
glm::vec3 light_position;

// Light objects. Now OOP.
AmbientLight aLight(
	glm::vec3(1.0f, 1.0f, 1.0f),
	0.5f);

PointLight pLight(
	glm::vec3(16.5f, 0.0f, -20.0f),	// Position.
	10.0f,									// Range.
	1.0f, 4.5f, 75.0f,				// Constant, Linear, Exponent.   
	glm::vec3(0.5f, 1.0f, 0.5f),		// Diffuse colour.
	1.0f);								// Diffuse strength.

Material mat = {1.0f, 32}; // Alternate way to construct an object.

// Camera
float scale = 1.0f, angle = 0.0f;
glm::vec3 position, frontVec, worldUp, upVec, rightVec; // Set by function
GLfloat pitch, yaw;
int lastX, lastY;

// Height params. Made it separated for better testing
float wallHeight = 5.0f;
float hedgeHeight = wallHeight / 2;

// Geometry
Grid g_grid(50);
Cube c_Platform(3.0f, 0.2f, 4.0f);
Cube Hedges[39] =
{
	/*
	* Their positions are delegated as { posX, posY, posZ }
	* Don't even try to change something, I was calculating
	* that 2 hours... so, 39 hedges
	* btw, height is = wallHeight / 2
	* 
	* maze size is 20 x 32
	* 
	*/

	// outline walls

	{1.0f, hedgeHeight, 24.0f},
	{15.0f, hedgeHeight, 1.0f},
	{14.0f, hedgeHeight, 1.0f},
	{1.0f, hedgeHeight, 21.0f},
	{33.0f, hedgeHeight, 1.0f},

	// ---
	// maze

	{1.0f, hedgeHeight, 2.0f},
	{1.0f, hedgeHeight, 8.0f},
	{8.0f, hedgeHeight, 1.0f},
	{1.0f, hedgeHeight, 8.0f},
	{5.0f, hedgeHeight, 1.0f},

	{3.0f, hedgeHeight, 1.0f},
	{1.0f, hedgeHeight, 3.0f},
	{7.0f, hedgeHeight, 1.0f},
	{1.0f, hedgeHeight, 4.0f},
	{1.0f, hedgeHeight, 4.0f},

	{1.0f, hedgeHeight, 4.0f},
	{1.0f, hedgeHeight, 4.0f},
	{1.0f, hedgeHeight, 3.0f},
	{1.0f, hedgeHeight, 1.0f},
	{1.0f, hedgeHeight, 1.0f},

	{1.0f, hedgeHeight, 1.0f},
	{1.0f, hedgeHeight, 4.0f},
	{1.0f, hedgeHeight, 2.0f},
	{8.0f, hedgeHeight, 1.0f},
	{9.0f, hedgeHeight, 1.0f},

	{1.0f, hedgeHeight, 4.0f},
	{5.0f, hedgeHeight, 1.0f},
	{11.0f, hedgeHeight, 1.0f},
	{6.0f, hedgeHeight, 1.0f},
	{1.0f, hedgeHeight, 8.0f},

	{6.0f, hedgeHeight, 1.0f},
	{1.0f, hedgeHeight, 7.0f},
	{1.0f, hedgeHeight, 4.0f},
	{1.0f, hedgeHeight, 4.0f},
	{5.0f, hedgeHeight, 1.0f},

	{1.0f, hedgeHeight, 6.0f},
	{6.0f, hedgeHeight, 1.0f},
	{10.0f, hedgeHeight, 1.0f},
	{1.0f, hedgeHeight, 10.0f}
};

Cube Walls[5] =
{
	{1.0f, 5.0f, 30.0f},
	{16.0f, 5.0f, 1.0f},
	{16.0f, 5.0f, 1.0f},
	{1.0f, 5.0f, 30.0f},
	{40.0f, 5.0f, 1.0f}
};

Cube MerlonsAndCrenels[8] =
{
	{0.1f, 0.5f, 31.0f},
	{0.1f, 0.5f, 29.0f},
	{40.0f, 0.5f, 0.1f},
	{38.2f, 0.5f, 0.1f},
	{0.1f, 0.5f, 31.0f},
	{0.1f, 0.5f, 29.0f},
	{40.0f, 0.5f, 0.1f},
	{38.2f, 0.5f, 0.1f},
};

Cube Crenels_0[9] =
{
	{0.1f, 0.3f, 3.0f},
	{0.1f, 0.3f, 3.0f},
	{0.1f, 0.3f, 3.0f},
	{0.1f, 0.3f, 3.0f},
	{0.1f, 0.3f, 3.0f},
	{0.1f, 0.3f, 3.0f},
	{0.1f, 0.3f, 3.0f},
	{0.1f, 0.3f, 3.0f},
	{0.1f, 0.3f, 3.0f}
};

Cube Crenels_1[13] =
{
	{2.6f, 0.3f, 0.1f},
	{2.6f, 0.3f, 0.1f},
	{2.6f, 0.3f, 0.1f},
	{2.6f, 0.3f, 0.1f},
	{2.6f, 0.3f, 0.1f},
	{2.6f, 0.3f, 0.1f},
	{2.6f, 0.3f, 0.1f},
	{2.6f, 0.3f, 0.1f},
	{2.6f, 0.3f, 0.1f},
	{2.6f, 0.3f, 0.1f},
	{2.6f, 0.3f, 0.1f},
	{2.6f, 0.3f, 0.1f},
	{2.6f, 0.3f, 0.1f}
};


Cone Roofs[4]
{
	8,
	8,
	8,
	8
};

Prism Towers[4]
{
	8,
	8,
	8,
	8
};


void timer( int ); // idk how, but it works

void resetView()
{
	// Setting camera 
	position = glm::vec3(17.0f, 2.0f, -27.0);
	frontVec = glm::vec3(0.0f, 0.0f, 0.0f);
	worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	pitch = 0.0f;
	yaw = -45.0f;
}


void init( void )
{

	/* Alex did it, don't change till next line-comment */
	srand(static_cast <unsigned>(time(nullptr)));
	

	vertexShaderId = setShader("vertex", "cube.vert");
	fragmentShaderId = setShader("fragment", "cube.frag");
	program = glCreateProgram();
	glAttachShader(program, vertexShaderId);
	glAttachShader(program, fragmentShaderId);
	glLinkProgram(program);
	glUseProgram(program);
	modelID = glGetUniformLocation(program, "model");
	viewID = glGetUniformLocation(program, "view");
	projID = glGetUniformLocation(program, "projection");
	// Projection matrix : 45∞ Field of View, 1:1 ratio, display range : 0.1 unit <-> 100 units
	Projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);
	// Projection = glm::ortho(-3.0f, 3.0f, -3.0f, 3.0f, 0.0f, 100.0f);
	// Camera matrix
	resetView();
	// Image loading.
	stbi_set_flip_vertically_on_load(true);

	/* --- --- */

	/* Loading images, just copy-paste if you need one more */

	// Loading image_1.
	unsigned char * image = stbi_load("Ground.jpg", &width, &height, &bitDepth, 0);
	if ( !image ) cout << "Unable to load file!" << endl;
	glGenTextures(1, &blankID);
	glBindTexture(GL_TEXTURE_2D, blankID);
	// Note: image types with native transparency will need to be GL_RGBA instead of GL_RGB.
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
		GL_UNSIGNED_BYTE, image);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(image);
	// End image_1.


	// Load image_2.
	image = stbi_load("Platform.jpg", &width, &height, &bitDepth, 0);
	if ( !image ) cout << "Unable to load file!" << endl;
	glGenTextures(1, &platformID);
	glBindTexture(GL_TEXTURE_2D, platformID);
	// Note: image types with native transparency will need to be GL_RGBA instead of GL_RGB.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
				 GL_UNSIGNED_BYTE, image);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(image);
	// End image_2.


	// Load image_3.
	image = stbi_load("Foliage.jpg", &width, &height, &bitDepth, 0);
	if ( !image ) cout << "Unable to load file!" << endl;
	glGenTextures(1, &hedgeID);
	glBindTexture(GL_TEXTURE_2D, hedgeID);
	// Note: image types with native transparency will need to be GL_RGBA instead of GL_RGB.
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
				 GL_UNSIGNED_BYTE, image);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(image);
	// End image_3.
	

	// Load image_4.
	image = stbi_load("Bricks.jpg", &width, &height, &bitDepth, 0);
	if ( !image ) cout << "Unable to load file!" << endl;
	glGenTextures(1, &wallID);
	glBindTexture(GL_TEXTURE_2D, wallID);
	// Note: image types with native transparency will need to be GL_RGBA instead of GL_RGB.
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
				 GL_UNSIGNED_BYTE, image);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(image);
	// End image_4.
 
	// Load image_5.
	image = stbi_load("Gravel.jpg", &width, &height, &bitDepth, 0);
	if ( !image ) cout << "Unable to load file!" << endl;
	glGenTextures(1, &wallTopID);
	glBindTexture(GL_TEXTURE_2D, wallTopID);
	// Note: image types with native transparency will need to be GL_RGBA instead of GL_RGB.
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
				 GL_UNSIGNED_BYTE, image);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(image);
	// End image_5.


	// Load image_6.
	image = stbi_load("Tall.jpg", &width, &height, &bitDepth, 0);
	if ( !image ) cout << "Unable to load file!" << endl;
	glGenTextures(1, &towerID);
	glBindTexture(GL_TEXTURE_2D, towerID);
	// Note: image types with native transparency will need to be GL_RGBA instead of GL_RGB.
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
				 GL_UNSIGNED_BYTE, image);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(image);
	// End image_6.


	// Load image_7.
	image = stbi_load("Roof.jpg", &width, &height, &bitDepth, 0);
	if ( !image ) cout << "Unable to load file!" << endl;
	glGenTextures(1, &roofID);
	glBindTexture(GL_TEXTURE_2D, roofID);
	// Note: image types with native transparency will need to be GL_RGBA instead of GL_RGB.
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
				 GL_UNSIGNED_BYTE, image);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(image);
	// End image_7.


	glUniform1i(glGetUniformLocation(program, "texture0"), 0);
	glUniform1f(glGetUniformLocation(program, "mat.specularStrength"), mat.specularStrength);
	glUniform1f(glGetUniformLocation(program, "mat.shininess"), mat.shininess);
	glUniform3f(glGetUniformLocation(program, "aLight.base.diffuseColour"), aLight.diffuseColour.x, aLight.diffuseColour.y, aLight.diffuseColour.z);
	glUniform1f(glGetUniformLocation(program, "aLight.base.diffuseStrength"), aLight.diffuseStrength);
	glUniform3f(glGetUniformLocation(program, "pLight.base.diffuseColour"), pLight.diffuseColour.x, pLight.diffuseColour.y, pLight.diffuseColour.z);
	glUniform1f(glGetUniformLocation(program, "pLight.base.diffuseStrength"), pLight.diffuseStrength);
	glUniform3f(glGetUniformLocation(program, "pLight.position"), pLight.position.x, pLight.position.y, pLight.position.z);
	glUniform1f(glGetUniformLocation(program, "pLight.constant"), pLight.constant);
	glUniform1f(glGetUniformLocation(program, "pLight.linear"), pLight.linear);
	glUniform1f(glGetUniformLocation(program, "pLight.exponent"), pLight.exponent);

	// All VAO/VBO data now in Shape.h, but just do it again.
	g_grid.BufferShape();
	c_Platform.BufferShape();
	for ( int i = 0; i < 39; i++ )
		Hedges[i].BufferShape();

	for ( int i = 0; i < 5; i++ )
		Walls[i].BufferShape();

	for ( int i = 0; i < 8; i++ )
		MerlonsAndCrenels[i].BufferShape();

	for ( int i = 0; i < 9; i++ )
		Crenels_0[i].BufferShape();

	for ( int i = 0; i < 13; i++ )
		Crenels_1[i].BufferShape();

	for ( int i = 0; i < 4; i++ )
		Towers[i].BufferShape();

	for ( int i = 0; i < 4; i++ )
		Roofs[i].BufferShape();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	timer(0);
}


/*
* 
* calculate view
* 
*/
void calculateView()
{
	frontVec.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	frontVec.y = sin(glm::radians(pitch));
	frontVec.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	frontVec = normalize(frontVec);
	rightVec = normalize(cross(frontVec, worldUp));
	upVec = normalize(cross(rightVec, frontVec));
	View = lookAt(
		position,            // Camera position
		position + frontVec, // Look target
		upVec);              // Up vector
}


/*
* 
* transformModel
* 
*/
void transformObject( glm::vec3 scale, glm::vec3 rotationAxis,
					  float rotationAngle, glm::vec3 translation )
{
	glm::mat4 Model;
	Model = glm::mat4(1.0f);
	Model = translate(Model, translation);
	Model = glm::rotate(Model, glm::radians(rotationAngle), rotationAxis);
	Model = glm::scale(Model, scale);
	// We must now update the View.
	calculateView();
	pLight.position += light_position;
	glUniform3f(glGetUniformLocation(program, "pLight.position"),
				pLight.position.x, pLight.position.y, pLight.position.z);
	glUniformMatrix4fv(modelID, 1, GL_FALSE, &Model[0][0]);
	glUniformMatrix4fv(viewID, 1, GL_FALSE, &View[0][0]);
	glUniformMatrix4fv(projID, 1, GL_FALSE, &Projection[0][0]);
}


/*
* 
* display
* 
*/
void display( void )
{
	/*
	* 
	* The worst implementation, I know, but I don't have time for something'
	* better, and, anyway, I won't deal with this project in future, so...
	* 
	*/

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindTexture(GL_TEXTURE_2D, blankID); // Use this texture for all shapes.
	// Grid.
	transformObject(glm::vec3(1.0f, 1.0f, 1.0f), X_AXIS, -90.0f,
					glm::vec3(0.0f, 0.0f, 0.0f));
	g_grid.DrawShape(GL_TRIANGLES);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Platform

	glBindTexture(GL_TEXTURE_2D, platformID);
	transformObject(glm::vec3(3.0f, 0.2f, 4.0), X_AXIS, 0.0,
					glm::vec3(15.0f, 0.0f, -29.0f));
	c_Platform.DrawShape(GL_TRIANGLES);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Hedges

	glBindTexture(GL_TEXTURE_2D, hedgeID);
	transformObject(Hedges[0].getCubePoints(), Y_AXIS, 0.0,
					glm::vec3(8.0, 0.0f, -33.0f));
	Hedges[0].DrawShape(GL_TRIANGLES);
	transformObject(Hedges[1].getCubePoints(), Y_AXIS, 0.0,
					glm::vec3(8.0, 0.0f, -34.0f));
	Hedges[1].DrawShape(GL_TRIANGLES);
	transformObject(Hedges[2].getCubePoints(), X_AXIS, 0.0,
					glm::vec3(27.0, 0.0f, -34.0f));
	Hedges[2].DrawShape(GL_TRIANGLES);
	transformObject(Hedges[3].getCubePoints(), Y_AXIS, 0.0,
					glm::vec3(41.0, 0.0f, -34.0f));
	Hedges[3].DrawShape(GL_TRIANGLES);
	transformObject(Hedges[4].getCubePoints(), X_AXIS, 0.0,
					glm::vec3(9.0, 0.0f, -13.0f));
	Hedges[4].DrawShape(GL_TRIANGLES);
	transformObject(Hedges[5].getCubePoints(), X_AXIS, 0.0,
					glm::vec3(15.0, 0.0f, -20.0f));
	Hedges[5].DrawShape(GL_TRIANGLES);
	transformObject(Hedges[6].getCubePoints(), Y_AXIS, 0.0,
					glm::vec3(11.0, 0.0f, -31.0f));
	Hedges[6].DrawShape(GL_TRIANGLES);
	transformObject(Hedges[7].getCubePoints(), X_AXIS, 0.0,
					glm::vec3(12.0, 0.0f, -31.0f));
	Hedges[7].DrawShape(GL_TRIANGLES);
	transformObject(Hedges[8].getCubePoints(), Y_AXIS, 0.0,
					glm::vec3(20.0, 0.0f, -31.0f));
	Hedges[8].DrawShape(GL_TRIANGLES);
	transformObject(Hedges[9].getCubePoints(), X_AXIS, 0.0,
					glm::vec3(11.0, 0.0f, -23.0f));
	Hedges[9].DrawShape(GL_TRIANGLES);
	transformObject(Hedges[10].getCubePoints(), X_AXIS, 0.0,
					glm::vec3(18.0, 0.0f, -23.0f));
	Hedges[10].DrawShape(GL_TRIANGLES);
	transformObject(Hedges[11].getCubePoints(), Y_AXIS, 0.0,
					glm::vec3(19.0, 0.0f, -22.0f));
	Hedges[11].DrawShape(GL_TRIANGLES);
	transformObject(Hedges[12].getCubePoints(), X_AXIS, 0.0,
					glm::vec3(11.0, 0.0f, -20.0f));
	Hedges[12].DrawShape(GL_TRIANGLES);
	transformObject(Hedges[13].getCubePoints(), Y_AXIS, 0.0,
					glm::vec3(11.0, 0.0f, -17.0f));
	Hedges[13].DrawShape(GL_TRIANGLES);
	transformObject(Hedges[14].getCubePoints(), Y_AXIS, 0.0,
					glm::vec3(14.0, 0.0f, -17.0f));
	Hedges[14].DrawShape(GL_TRIANGLES);
	transformObject(Hedges[15].getCubePoints(), Y_AXIS, 0.0,
					glm::vec3(17.0, 0.0f, -19.0f));
	Hedges[15].DrawShape(GL_TRIANGLES);
	transformObject(Hedges[16].getCubePoints(), Y_AXIS, 0.0,
					glm::vec3(20.0, 0.0f, -17.0f));
	Hedges[16].DrawShape(GL_TRIANGLES);
	transformObject(Hedges[17].getCubePoints(), Y_AXIS, 0.0,
					glm::vec3(23.0, 0.0f, -19.0f));
	Hedges[17].DrawShape(GL_TRIANGLES);
	transformObject(Hedges[18].getCubePoints(), X_AXIS, 0.0,
					glm::vec3(24.0, 0.0f, -14.0f));
	Hedges[18].DrawShape(GL_TRIANGLES);
	transformObject(Hedges[19].getCubePoints(), X_AXIS, 0.0,
					glm::vec3(24.0, 0.0f, -15.0f));
	Hedges[19].DrawShape(GL_TRIANGLES);
	transformObject(Hedges[20].getCubePoints(), X_AXIS, 0.0,
					glm::vec3(25.0, 0.0f, -15.0f));
	Hedges[20].DrawShape(GL_TRIANGLES);
	transformObject(Hedges[21].getCubePoints(), Y_AXIS, 0.0,
					glm::vec3(28.0, 0.0f, -17.0f));
	Hedges[21].DrawShape(GL_TRIANGLES);
	transformObject(Hedges[22].getCubePoints(), Y_AXIS, 0.0,
					glm::vec3(38.0, 0.0f, -15.0f));
	Hedges[22].DrawShape(GL_TRIANGLES);
	transformObject(Hedges[23].getCubePoints(), X_AXIS, 0.0,
					glm::vec3(30.0, 0.0f, -16.0f));
	Hedges[23].DrawShape(GL_TRIANGLES);
	transformObject(Hedges[24].getCubePoints(), X_AXIS, 0.0,
					glm::vec3(28.0, 0.0f, -18.0f));
	Hedges[24].DrawShape(GL_TRIANGLES);
	transformObject(Hedges[25].getCubePoints(), Y_AXIS, 0.0,
					glm::vec3(38.0, 0.0f, -19.0f));
	Hedges[25].DrawShape(GL_TRIANGLES);
	transformObject(Hedges[26].getCubePoints(), X_AXIS, 0.0,
					glm::vec3(34.0, 0.0f, -20.0f));
	Hedges[26].DrawShape(GL_TRIANGLES);
	transformObject(Hedges[27].getCubePoints(), X_AXIS, 0.0,
					glm::vec3(21.0, 0.0f, -20.0f));
	Hedges[27].DrawShape(GL_TRIANGLES);
	transformObject(Hedges[28].getCubePoints(), X_AXIS, 0.0,
					glm::vec3(23.0, 0.0f, -22.0f));
	Hedges[28].DrawShape(GL_TRIANGLES);
	transformObject(Hedges[29].getCubePoints(), Y_AXIS, 0.0,
					glm::vec3(23.0, 0.0f, -30.0f));
	Hedges[29].DrawShape(GL_TRIANGLES);
	transformObject(Hedges[30].getCubePoints(), X_AXIS, 0.0,
					glm::vec3(21.0, 0.0f, -31.0f));
	Hedges[30].DrawShape(GL_TRIANGLES);
	transformObject(Hedges[31].getCubePoints(), Y_AXIS, 0.0,
					glm::vec3(26.0, 0.0f, -30.0f));
	Hedges[31].DrawShape(GL_TRIANGLES);
	transformObject(Hedges[32].getCubePoints(), Y_AXIS, 0.0,
					glm::vec3(29.0, 0.0f, -24.0f));
	Hedges[32].DrawShape(GL_TRIANGLES);
	transformObject(Hedges[33].getCubePoints(), Y_AXIS, 0.0,
					glm::vec3(32.0, 0.0f, -25.0f));
	Hedges[33].DrawShape(GL_TRIANGLES);
	transformObject(Hedges[34].getCubePoints(), X_AXIS, 0.0,
					glm::vec3(30.0, 0.0f, -26.0f));
	Hedges[34].DrawShape(GL_TRIANGLES);
	transformObject(Hedges[35].getCubePoints(), Y_AXIS, 0.0,
					glm::vec3(35.0, 0.0f, -26.0f));
	Hedges[35].DrawShape(GL_TRIANGLES);
	transformObject(Hedges[36].getCubePoints(), X_AXIS, 0.0,
					glm::vec3(32.0, 0.0f, -28.0f));
	Hedges[36].DrawShape(GL_TRIANGLES);
	transformObject(Hedges[37].getCubePoints(), X_AXIS, 0.0,
					glm::vec3(29.0, 0.0f, -31.0f));
	Hedges[37].DrawShape(GL_TRIANGLES);
	transformObject(Hedges[38].getCubePoints(), Y_AXIS, 0.0,
					glm::vec3(38.0, 0.0f, -30.0f));
	Hedges[38].DrawShape(GL_TRIANGLES);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindTexture(GL_TEXTURE_2D, wallID);

	// Walls

	transformObject(glm::vec3(1.0f, 5.0f, 30.0f), Y_AXIS, 0.0f,
					glm::vec3(5.0f, 0.0f, -39.0f));
	Walls[0].DrawShape(GL_TRIANGLES);
	transformObject(glm::vec3(18.0f, 5.0f, 1.0f), X_AXIS, 0.0,
					glm::vec3(5.0f, 0.0f, -40.0f));
	Walls[1].DrawShape(GL_TRIANGLES);
	transformObject(glm::vec3(18.0f, 5.0f, 1.0f), X_AXIS, 0.0,
					glm::vec3(27.0f, 0.0f, -40.0f));
	Walls[2].DrawShape(GL_TRIANGLES);
	transformObject(glm::vec3(1.0f, 5.0f, 30.0f), X_AXIS, 0.0,
					glm::vec3(44.0f, 0.0f, -39.0f));
	Walls[3].DrawShape(GL_TRIANGLES);
	transformObject(glm::vec3(40.0f, 5.0f, 1.0f), X_AXIS, 0.0,
					glm::vec3(5.0f, 0.0f, -10.0f));
	Walls[4].DrawShape(GL_TRIANGLES);

	// Merlons And Crenels

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindTexture(GL_TEXTURE_2D, wallTopID);
	transformObject(MerlonsAndCrenels[0].getCubePoints(), Y_AXIS, 0.0f,
					glm::vec3(5.0f, 5.0f, -40.0f));
	MerlonsAndCrenels[0].DrawShape(GL_TRIANGLES);
	transformObject(MerlonsAndCrenels[1].getCubePoints(), Y_AXIS, 0.0f,
					glm::vec3(5.9f, 5.0f, -39.0f));
	MerlonsAndCrenels[1].DrawShape(GL_TRIANGLES);
	transformObject(MerlonsAndCrenels[2].getCubePoints(), Y_AXIS, 0.0f,
					glm::vec3(5.0f, 5.0f, -40.0f));
	MerlonsAndCrenels[2].DrawShape(GL_TRIANGLES);
	transformObject(MerlonsAndCrenels[3].getCubePoints(), Y_AXIS, 0.0f,
					glm::vec3(5.9f, 5.0f, -39.1f));
	MerlonsAndCrenels[3].DrawShape(GL_TRIANGLES);
	transformObject(MerlonsAndCrenels[4].getCubePoints(), Y_AXIS, 0.0f,
					glm::vec3(44.9f, 5.0f, -40.0f));
	MerlonsAndCrenels[4].DrawShape(GL_TRIANGLES);
	transformObject(MerlonsAndCrenels[5].getCubePoints(), Y_AXIS, 0.0f,
					glm::vec3(44.0f, 5.0f, -39.0f));
	MerlonsAndCrenels[5].DrawShape(GL_TRIANGLES);
	transformObject(MerlonsAndCrenels[6].getCubePoints(), Y_AXIS, 0.0f,
					glm::vec3(5.0f, 5.0f, -9.1f));
	MerlonsAndCrenels[6].DrawShape(GL_TRIANGLES);
	transformObject(MerlonsAndCrenels[7].getCubePoints(), Y_AXIS, 0.0f,
					glm::vec3(5.9f, 5.0f, -10.0f));

	MerlonsAndCrenels[7].DrawShape(GL_TRIANGLES);

	// Crenels

	float j = 0.0f;
	for ( int i = 0; i < 9; i++ )
	{
		transformObject(Crenels_0[i].getCubePoints(), Y_AXIS, 0.0f,
						glm::vec3(5.0f, 5.5f, -40.0f + j)); // WallTop 0
		Crenels_0[i].DrawShape(GL_TRIANGLES);
		j += 3.5f;
	}
	j = 0.0f;

	for ( int i = 0; i < 9; i++ )
	{
		transformObject(Crenels_0[i].getCubePoints(), Y_AXIS, 0.0f,
						glm::vec3(5.9f, 5.5f, -40.0f + j)); // WallTop 1
		Crenels_0[i].DrawShape(GL_TRIANGLES);
		j += 3.5f;
	}
	j = 0.0f;

	for ( int i = 0; i < 9; i++ )
	{
		transformObject(Crenels_0[i].getCubePoints(), Y_AXIS, 0.0f,
						glm::vec3(44.9f, 5.5f, -40.0f + j)); // WallTop 4
		Crenels_0[i].DrawShape(GL_TRIANGLES);
		j += 3.5f;
	}
	j = 0.0f;

	for ( int i = 0; i < 9; i++ )
	{
		transformObject(Crenels_0[i].getCubePoints(), Y_AXIS, 0.0f,
						glm::vec3(44.0f, 5.5f, -40.0f + j)); // WallTop 5
		Crenels_0[i].DrawShape(GL_TRIANGLES);
		j += 3.5f;
	}
	j = 0.0f;

	for ( int i = 0; i < 13; i++ )
	{
		transformObject(Crenels_1[i].getCubePoints(), Y_AXIS, 0.0f,
						glm::vec3(5.0f + j, 5.5f, -40.0f)); // WallTop 2
		Crenels_1[i].DrawShape(GL_TRIANGLES);
		j += Crenels_1[i].getCubePoints().x + 0.5f;
	}
	j = 0.0f;

	for ( int i = 0; i < 13; i++ )
	{
		transformObject(Crenels_1[i].getCubePoints(), Y_AXIS, 0.0f,
						glm::vec3(5.0f + j, 5.5f, -39.1f)); // WallTop 3
		Crenels_1[i].DrawShape(GL_TRIANGLES);
		j += Crenels_1[i].getCubePoints().x + 0.5f;
	}
	j = 0.0f;

	for ( int i = 0; i < 13; i++ )
	{
		transformObject(Crenels_1[i].getCubePoints(), Y_AXIS, 0.0f,
						glm::vec3(5.0f + j, 5.5f, -9.1f)); // WallTop 6
		Crenels_1[i].DrawShape(GL_TRIANGLES);
		j += Crenels_1[i].getCubePoints().x + 0.5f;
	}
	j = 0.0f;

	for ( int i = 0; i < 13; i++ )
	{
		transformObject(Crenels_1[i].getCubePoints(), Y_AXIS, 0.0f,
						glm::vec3(5.0f + j, 5.5f, -10.0f)); // WallTop 7
		Crenels_1[i].DrawShape(GL_TRIANGLES);
		j += Crenels_1[i].getCubePoints().x + 0.5f;
	}

	// Prism.
	glBindTexture(GL_TEXTURE_2D, towerID);
	transformObject(glm::vec3(2.0f, 3.5f, 2.0f), X_AXIS, 0.0f,
					glm::vec3(4.5f, 5.0f, -10.53f));
	Towers[0].DrawShape(GL_TRIANGLES);
	transformObject(glm::vec3(2.0f, 3.5f, 2.0f), X_AXIS, 0.0f,
					glm::vec3(43.3f, 5.0f, -10.53f));
	Towers[1].DrawShape(GL_TRIANGLES);
	transformObject(glm::vec3(2.0f, 3.5f, 2.0f), X_AXIS, 0.0f,
					glm::vec3(43.3f, 5.0f, -40.35f));
	Towers[2].DrawShape(GL_TRIANGLES);
	transformObject(glm::vec3(2.0f, 3.5f, 2.0f), X_AXIS, 0.0f,
					glm::vec3(4.5f, 5.0f, -40.35f));
	Towers[3].DrawShape(GL_TRIANGLES);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	// Roof

	glBindTexture(GL_TEXTURE_2D, roofID);
	transformObject(glm::vec3(3.0f, 4.0f, 3.0f), X_AXIS, 0.0f,
					glm::vec3(4.0f, 8.5f, -11.0f));
	Roofs[0].DrawShape(GL_TRIANGLES);
	transformObject(glm::vec3(3.0f, 4.0f, 3.0f), X_AXIS, 0.0f,
					glm::vec3(43.0f, 8.5f, -11.0f));
	Roofs[1].DrawShape(GL_TRIANGLES);
	transformObject(glm::vec3(3.0f, 4.0f, 3.0f), X_AXIS, 0.0f,
					glm::vec3(43.0f, 8.5f, -40.9f));
	Roofs[2].DrawShape(GL_TRIANGLES);
	transformObject(glm::vec3(3.0f, 4.0f, 3.0f), X_AXIS, 0.0f,
					glm::vec3(4.0f, 8.5f, -40.9f));
	Roofs[3].DrawShape(GL_TRIANGLES);

	glClearColor(0.63f, 0.89f, 0.72f, 1.0f); // bg-color
	glutSwapBuffers(); // Buffer
}


void idle() { glutPostRedisplay(); }


void parseKeys()
{
	if ( keys & KEY_FORWARD )
		position += frontVec * MOVESPEED;
	if ( keys & KEY_BACKWARD )
		position -= frontVec * MOVESPEED;
	if ( keys & KEY_LEFT )
		position -= rightVec * MOVESPEED;
	if ( keys & KEY_RIGHT )
		position += rightVec * MOVESPEED;
	if ( keys & KEY_UP )
		position += upVec * MOVESPEED;
	if ( keys & KEY_DOWN )
		position -= upVec * MOVESPEED;
}


void timer( int )
{
	// Tick of the frame.
	// Get first timestamp
	int start = glutGet(GLUT_ELAPSED_TIME);
	// Update call
	parseKeys();
	// Display call
	glutPostRedisplay();
	// Calling next tick
	int end = glutGet(GLUT_ELAPSED_TIME);
	glutTimerFunc(( 1000 / FPS ) - ( end - start ), timer, 0);
}

// Keyboard ( Varmilo the best [ gift me pls ] )
void keyDown( unsigned char key, int x, int y )
{
	switch ( key )
	{
		case 27 :
			exit(0);
			break;
		case 'w' :
			if ( !( keys & KEY_FORWARD ) )
				keys |= KEY_FORWARD;
			break;
		case 's' :
			if ( !( keys & KEY_BACKWARD ) )
				keys |= KEY_BACKWARD;
			break;
		case 'a' :
			if ( !( keys & KEY_LEFT ) )
				keys |= KEY_LEFT;
			break;
		case 'd' :
			if ( !( keys & KEY_RIGHT ) )
				keys |= KEY_RIGHT;
			break;
		case 'r' :
			if ( !( keys & KEY_UP ) )
				keys |= KEY_UP;
			break;
		case 'f' :
			if ( !( keys & KEY_DOWN ) )
				keys |= KEY_DOWN;
			break;
			// Camera controls
		case 'i' :
			light_position.z -= 1.0f * MOVESPEED;
			break;
		case 'j' :
			light_position.x -= 1.0f * MOVESPEED;
			break;
		case 'k' :
			light_position.z += 1.0f * MOVESPEED;
			break;
		case 'l' :
			light_position.x += 1.0f * MOVESPEED;
			break;
		default :
			break;
	}
}


void keyDownSpec( int key, int x, int y )
{
	switch ( key )
	{
		case GLUT_KEY_UP :
			if ( !( keys & KEY_FORWARD ) )
				keys |= KEY_FORWARD;
			break;
		case GLUT_KEY_DOWN : // Down arrow.
			if ( !( keys & KEY_BACKWARD ) )
				keys |= KEY_BACKWARD;
			break;
		default :
			break;
	}
}


void keyUp( unsigned char key, int x, int y )
{
	switch ( key )
	{
		case 'w' :
			keys &= ~KEY_FORWARD;
			break;
		case 's' :
			keys &= ~KEY_BACKWARD;
			break;
		case 'a' :
			keys &= ~KEY_LEFT;
			break;
		case 'd' :
			keys &= ~KEY_RIGHT;
			break;
		case 'r' :
			keys &= ~KEY_UP;
			break;
		case 'f' :
			keys &= ~KEY_DOWN;
			break;
		case 'i' :
			light_position.z += 1.0f * MOVESPEED;
			break;
		case 'j' :
			light_position.x += 1.0f * MOVESPEED;
			break;
		case 'k' :
			light_position.z -= 1.0f * MOVESPEED;
			break;
		case 'l' :
			light_position.x -= 1.0f * MOVESPEED;
			break;
		case ' ' :
			resetView();
			break;
		default :
			break;
	}
}


void keyUpSpec( int key, int x, int y )
// x and y is mouse location upon key press.
{
	switch ( key )
	{
		case GLUT_KEY_UP :
			keys &= ~KEY_FORWARD; // keys = keys & ~KEY_FORWARD
			break;
		case GLUT_KEY_DOWN :
			keys &= ~KEY_BACKWARD;
			break;
		default :
			break;
	}
}


void mouseMove( int x, int y )
{
	if ( keys & KEY_MOUSECLICKED )
	{
		pitch += ( y - lastY ) * TURNSPEED;
		yaw -= ( x - lastX ) * TURNSPEED;
		lastY = y;
		lastX = x;
	}
}


void mouseClick( int btn, int state, int x, int y )
{
	if ( state == 0 )
	{
		lastX = x;
		lastY = y;
		keys |= KEY_MOUSECLICKED;
		glutSetCursor(GLUT_CURSOR_NONE);
	}
	else
	{
		keys &= ~KEY_MOUSECLICKED;
		glutSetCursor(GLUT_CURSOR_INHERIT);
	}
}


//---------------------------------------------------------------------
//
// clean
//
void clean()
{
	cout << "Cleaning up!" << endl;
	glDeleteTextures(1, &blankID);
}


//---------------------------------------------------------------------
//
// main
//
int main( int argc, char ** argv )
{
	//Before we can open a window ...
	glutInit(&argc, argv);
	glutInitDisplayMode(
		GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_MULTISAMPLE);
	glutSetOption(GLUT_MULTISAMPLE, 8);
	glutInitWindowSize(1280, 800);
	//the top-left corner of the display
	glutInitWindowPosition(10, 40);
	glutCreateWindow("GAME2012_Final_GastelloAndrii");
	glewInit(); //Initializes the glew and prepares the drawing pipeline.
	init();     // Our own custom function.
	glutDisplayFunc(display);
	glutKeyboardFunc(keyDown);
	glutSpecialFunc(keyDownSpec);
	glutKeyboardUpFunc(keyUp);
	glutSpecialUpFunc(keyUpSpec);
	glutMouseFunc(mouseClick);
	glutMotionFunc(mouseMove); // Requires click to register.
	atexit(clean);
	// This useful GLUT function calls specified function before exiting program. 
	glutMainLoop();
	return 0;
}
