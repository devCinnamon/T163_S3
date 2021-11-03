#pragma once
#ifndef __PLAY_SCENE__
#define __PLAY_SCENE__

#include "Scene.h"
#include "Plane.h"
#include "Player.h"
#include "Button.h"
#include "Label.h"
#include "ExampleObject.h"
#include <list>

class FuncSprite : public Sprite
{
public:
	FuncSprite();
	virtual ~FuncSprite();
	void draw();
	void update();
	void clean();
};

class PlayScene : public Scene
{
public:
	PlayScene();
	~PlayScene();

	// Scene LifeCycle Functions
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;
	virtual void handleEvents() override;
	virtual void start() override;

	void LaunchProjectile(float angle, float speed);
	glm::vec2 GetLaunchVector(float angle, float speed);

private:

	std::list<Sprite*> projectiles;

	// IMGUI Function
	void GUI_Function();
	std::string m_guiTitle;
	
	glm::vec2 m_mousePosition;
	
	// UI Items
	Button* m_pBackButton;
	Button* m_pNextButton;
	Label* m_pInstructionsLabel;

	// Game Physics vars
	float m_gameTime;

	glm::vec2 startPos = glm::vec2(100, 600);
	glm::vec2 currentVelocity = glm::vec2(0, 0);
	float launchAngleDeg = 60.0f;
	float elevationRate = 30;
	float speedAdjustmentRate = 600;
	float launchSpeed = 600;
	float groundPosition = 600;

	float massKg = 5;
	float accelerationGravity = 981;
	

};

#endif /* defined (__PLAY_SCENE__) */