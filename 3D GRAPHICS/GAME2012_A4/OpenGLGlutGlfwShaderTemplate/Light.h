#pragma once
#ifndef _LIGHT_H_
#define _LIGHT_H_
#include <GL/glew.h>
#include <glm/glm.hpp>

using namespace std;

struct Light
{
	glm::vec3 diffuseColour;
	GLfloat diffuseStrength;
	Light(glm::vec3 dCol, GLfloat dStr)
	{
		diffuseColour = dCol;
		diffuseStrength = dStr;
	}
};

struct AmbientLight : public Light
{
	AmbientLight(glm::vec3 aCol, GLfloat aStr) : Light(aCol, aStr) {}
};

struct DirectionalLight : public Light
{
	glm::vec3 origin;
	DirectionalLight(glm::vec3 ori, glm::vec3 dCol, GLfloat dStr)
		: Light(dCol, dStr), origin(ori) { }
};

struct PointLight : public Light
{
	glm::vec3 position; //= glm::vec3(0.0f, 0.0f, 0.0f);
	GLfloat constant, linear, quadratic;
	PointLight(glm::vec3 pos, GLfloat range, GLfloat con, GLfloat lin, GLfloat quad,
		glm::vec3 dCol, GLfloat dStr) : Light(dCol, dStr)
	{
		position = pos;
		constant = con; //= 1.0f
		linear = lin / range; //= 4.5f / range;
		quadratic = quad / (range * range); //= 75.0f / (range * range);
	}
};

struct SpotLight : public Light
{
	glm::vec3 position;
	glm::vec3 direction;
	GLfloat edge, edgeRad;
	SpotLight(glm::vec3 pos, glm::vec3 dCol, GLfloat dStr, glm::vec3 dir,
		GLfloat e) : Light(dCol, dStr)
	{
		position = pos;
		direction = -(glm::normalize(dir)); // Normally opposite but this allows it to be actual direction of cone.
		edge = e;
		edgeRad = cosf(glm::radians(edge));
	}
};

struct Material
{
	GLfloat specularStrength;
	GLfloat shininess;
};

#endif