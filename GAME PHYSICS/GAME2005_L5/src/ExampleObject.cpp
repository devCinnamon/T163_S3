#include "ExampleObject.h"
#include "TextureManager.h"
#include "Game.h"

ExampleObject::ExampleObject()
{
	TextureManager::Instance().load("../Assets/textures/ship3.png", "ship");

	auto size = TextureManager::Instance().getTextureSize("ship");
	setWidth(size.x);
	setHeight(size.y);
}

ExampleObject::~ExampleObject()
= default;

void ExampleObject::draw()
{
	TextureManager::Instance().draw("ship",
		getTransform()->position.x, getTransform()->position.y, 0, 255, true);
}

void ExampleObject::update()
{
}

void ExampleObject::clean()
{
}
