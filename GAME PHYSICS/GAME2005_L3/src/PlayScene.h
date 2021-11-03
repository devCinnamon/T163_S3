#pragma once
#ifndef __PLAY_SCENE__
#define __PLAY_SCENE__

#include "Button.h"
#include "Game.h"
#include "Label.h"
#include "Plane.h"
#include "Player.h"
#include "Scene.h"
#include "ship.h"

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
private:
	// IMGUI Function
	void GUI_Function();
	std::string m_guiTitle;
	
	glm::vec2 m_mousePosition;

	Plane* m_pPlaneSprite;
	Player* m_pPlayer;
	bool m_playerFacingRight;

	// UI Items
	Button* m_pBackButton;
	Button* m_pNextButton;
	Label* m_pInstructionsLabel;

	Ship* m_pShip{};

	// Physics Variables
	float m_gameTime;
	float m_angleDeg = 45;


	const float deg_to_radians = M_PI / 180.00;

	glm::vec2 startPos = glm::vec2(100, 400);
	glm::vec2 lastPos = glm::vec2(100, 400);
	float groundPos = 600.0f;

	float m_speedPerSec = 400;
	float accelerationGravityInPixels = 300;
	float elevationRate = 10;
	float SpeedAdjustmentRate = 10;

	int numPoints = 3000;
	float totalSimulationTimeElapsed = 30.0f;
};
#endif /* defined (__PLAY_SCENE__) */