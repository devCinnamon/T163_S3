#include "PlayScene.h"
#include "Game.h"
#include "EventManager.h"

// required for IMGUI
#include "imgui.h"
#include "imgui_sdl.h"
#include "Renderer.h"
#include "Util.h"



PlayScene::PlayScene()
{
	PlayScene::start();
}

PlayScene::~PlayScene()
= default;

void PlayScene::draw()
{
	drawDisplayList();

	SDL_SetRenderDrawColor(Renderer::Instance().getRenderer(), 255, 0, 0, 255);

	float vy = sin(-m_angleDeg * deg_to_radians) * m_speedPerSec;
	float vx = cos(-m_angleDeg * deg_to_radians) * m_speedPerSec;

	// Loop, draw lines between fixed number of points
	glm::vec2 lastPoint = startPos;
	glm::vec2 nextPoint;

	float simulationTime = 0.0f;
	float timeByStep = totalSimulationTimeElapsed / numPoints;

	// Exactly loop for evaluation points each step
	for (int i = 0; i < numPoints; i++)
	{
		nextPoint.x = startPos.x + vx * simulationTime;
		nextPoint.y = startPos.y + vy * simulationTime + 0.5 * accelerationGravityInPixels * pow(simulationTime, 2);

		SDL_RenderDrawLineF(Renderer::Instance().getRenderer(), lastPoint.x, lastPoint.y, nextPoint.x, nextPoint.y);

		lastPoint = nextPoint;

		simulationTime += timeByStep;
	}

	// Drawing Red Line of the trajectory without physics

	//SDL_SetRenderDrawColor(Renderer::Instance().getRenderer(), 255, 0, 0, 255);
	//SDL_RenderDrawLineF(Renderer::Instance().getRenderer(), startPos.x, startPos.y, startPos.x + vel.x, startPos.y + vel.y);

	// Drawing Ground Line
	
	SDL_SetRenderDrawColor(Renderer::Instance().getRenderer(), 0, 0, 0, 255);
	SDL_RenderDrawLine(Renderer::Instance().getRenderer(), 0, groundPos, 800, groundPos );

	SDL_SetRenderDrawColor(Renderer::Instance().getRenderer(), 255, 255, 255, 255);
	/* --- */
	
}

void PlayScene::update()
{
	// dependecies for a physics
	float dt = TheGame::Instance().getDeltaTime();
	m_gameTime += dt;

	updateDisplayList();

	m_pShip->getRigidBody()->acceleration.y = accelerationGravityInPixels;
	
	m_pShip->getRigidBody()->velocity += m_pShip->getRigidBody()->acceleration * dt;
	m_pShip->getTransform()->position += m_pShip->getRigidBody()->velocity * dt;

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_SPACE))
	{
		m_pShip->getTransform()->position = glm::vec2(startPos.x, startPos.y);

		m_pShip->getRigidBody()->velocity.x = cos(-m_angleDeg * deg_to_radians) * m_speedPerSec;
		m_pShip->getRigidBody()->velocity.y = sin(m_angleDeg * deg_to_radians) * -m_speedPerSec;
	}
}

void PlayScene::clean()
{
	removeAllChildren();
}

void PlayScene::handleEvents()
{
	EventManager::Instance().update();

	/*

	// handle player movement with GameController
	if (SDL_NumJoysticks() > 0)
	{
		if (EventManager::Instance().getGameController(0) != nullptr)
		{
			const auto deadZone = 10000;
			if (EventManager::Instance().getGameController(0)->LEFT_STICK_X > deadZone)
			{
				m_pPlayer->setAnimationState(PLAYER_RUN_RIGHT);
				m_playerFacingRight = true;
			}
			else if (EventManager::Instance().getGameController(0)->LEFT_STICK_X < -deadZone)
			{
				m_pPlayer->setAnimationState(PLAYER_RUN_LEFT);
				m_playerFacingRight = false;
			}
			else
			{
				if (m_playerFacingRight)
				{
					m_pPlayer->setAnimationState(PLAYER_IDLE_RIGHT);
				}
				else
				{
					m_pPlayer->setAnimationState(PLAYER_IDLE_LEFT);
				}
			}
		}
	}


	// handle player movement if no Game Controllers found
	if (SDL_NumJoysticks() < 1)
	{
		if (EventManager::Instance().isKeyDown(SDL_SCANCODE_A))
		{
			m_pPlayer->setAnimationState(PLAYER_RUN_LEFT);
			m_playerFacingRight = false;
		}
		else if (EventManager::Instance().isKeyDown(SDL_SCANCODE_D))
		{
			m_pPlayer->setAnimationState(PLAYER_RUN_RIGHT);
			m_playerFacingRight = true;
		}
		else
		{
			if (m_playerFacingRight)
			{
				m_pPlayer->setAnimationState(PLAYER_IDLE_RIGHT);
			}
			else
			{
				m_pPlayer->setAnimationState(PLAYER_IDLE_LEFT);
			}
		}
	}

	*/

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_ESCAPE))
	{
		TheGame::Instance().quit();
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_1))
	{
		TheGame::Instance().changeSceneState(START_SCENE);
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_2))
	{
		TheGame::Instance().changeSceneState(END_SCENE);
	}
}

void PlayScene::start()
{
	// Set GUI Title
	m_guiTitle = "Play Scene";

	/*
	
	// Plane Sprite
	m_pPlaneSprite = new Plane();
	addChild(m_pPlaneSprite);

	// Player Sprite
	m_pPlayer = new Player();
	addChild(m_pPlayer);
	m_playerFacingRight = true;

	*/

	// Ship Sprite
	m_pShip = new Ship();
	m_pShip->getTransform()->position = startPos; 
	addChild(m_pShip);
	
	// Back Button
	m_pBackButton = new Button("../Assets/textures/backButton.png", "backButton", BACK_BUTTON);
	m_pBackButton->getTransform()->position = glm::vec2(300.0f, 400.0f);
	m_pBackButton->addEventListener(CLICK, [&]()-> void
	{
		m_pBackButton->setActive(false);
		TheGame::Instance().changeSceneState(START_SCENE);
	});

	m_pBackButton->addEventListener(MOUSE_OVER, [&]()->void
	{
		m_pBackButton->setAlpha(128);
	});

	m_pBackButton->addEventListener(MOUSE_OUT, [&]()->void
	{
		m_pBackButton->setAlpha(255);
	});
	addChild(m_pBackButton);

	// Next Button
	m_pNextButton = new Button("../Assets/textures/nextButton.png", "nextButton", NEXT_BUTTON);
	m_pNextButton->getTransform()->position = glm::vec2(500.0f, 400.0f);
	m_pNextButton->addEventListener(CLICK, [&]()-> void
	{
		m_pNextButton->setActive(false);
		TheGame::Instance().changeSceneState(END_SCENE);
	});

	m_pNextButton->addEventListener(MOUSE_OVER, [&]()->void
	{
		m_pNextButton->setAlpha(128);
	});

	m_pNextButton->addEventListener(MOUSE_OUT, [&]()->void
	{
		m_pNextButton->setAlpha(255);
	});

	addChild(m_pNextButton);

	/* Instructions Label */
	m_pInstructionsLabel = new Label("Press the backtick (`) character to toggle Debug View", "Consolas");
	m_pInstructionsLabel->getTransform()->position = glm::vec2(Config::SCREEN_WIDTH * 0.5f, 500.0f);

	addChild(m_pInstructionsLabel);

	ImGuiWindowFrame::Instance().setGUIFunction(std::bind(&PlayScene::GUI_Function, this));
}

void PlayScene::GUI_Function()
{
	// Always open with a NewFrame
	ImGui::NewFrame();

	// See examples by uncommenting the following - also look at imgui_demo.cpp in the IMGUI filter
	//ImGui::ShowDemoWindow();
	
	ImGui::Begin("Lab 1", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoMove);

	// if(ImGui::Button("My Button")) { std::cout << "My Button Pressed" << std::endl; }

	ImGui::Separator();

	/*
	
	static float float3[3] = { 0.0f, 1.0f, 1.5f };
	if(ImGui::SliderFloat3("My Slider", float3, 0.0f, 2.0f))
	{
		std::cout << float3[0] << std::endl;
		std::cout << float3[1] << std::endl;
		std::cout << float3[2] << std::endl;
		std::cout << "---------------------------\n";
	}

	*/

	ImGui::Separator();

	ImGui::SliderFloat("Speed", &m_speedPerSec, 0.0f, 800.0f, "%.3f");
	ImGui::SliderFloat("Angle", &m_angleDeg, 0.0f, 800.0f, "%.3f");

	ImGui::SliderFloat("Gravity", &accelerationGravityInPixels, 0.0f, 900.80f, "%.5f");

	ImGui::SliderFloat("Simulation Time", &totalSimulationTimeElapsed, 0.0f, 60.0f, "%.3f");
	ImGui::SliderInt("Sample Points", &numPoints, 0, 10000, "%.1f");

	ImGui::Separator();

	ImGui::SliderFloat("Ground", &groundPos, 0.0f, 800.0f, "%1.0f");

	ImGui::Separator();
	

	ImGui::End();
}
