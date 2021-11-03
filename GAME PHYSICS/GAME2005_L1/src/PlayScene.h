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
	Ship* m_pShip1{};

	// Ships physics formulas
	float t;
	float f_a = 9, f_b = 54, f_c = 200, f_d = 200;

	float f_ship1Y;
	float f_ship1X;
};
#endif /* defined (__PLAY_SCENE__) */