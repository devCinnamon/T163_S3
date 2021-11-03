#include "PlayScene.h"
#include "Game.h"
#include "EventManager.h"

// required for IMGUI
#include "imgui.h"
#include "imgui_sdl.h"
#include "Renderer.h"
#include "Util.h"

//Solves for x for equations of the form ax^2 + bx + c = 0, where a != 0
// returns number of solutions (0, 1, or 2)
int quadraticFormula(double a, double b, double c, double& outSolution1, double& outSolution2)
{
	double determinant = (b * b) - (4 * a * c);
	double inverseTwoA = 1.0 / (2.0 * a);

	int numSolutions = 0;

	if (determinant < 0)
	{
		// imaginary roots.
		return numSolutions;
	}
	else if (determinant == 0)
	{
		// 1 root
		numSolutions = 1;
		outSolution1 = -b * inverseTwoA;
		outSolution2 = outSolution1;
	}
	else
	{
		// 2 roots
		numSolutions = 2;
		outSolution1 = (sqrt(determinant) - b) * inverseTwoA; // -b + det
		outSolution2 = (-b - sqrt(determinant)) * inverseTwoA; // -b - det
	}

	return numSolutions;
}

PlayScene::PlayScene()
{
	PlayScene::start();
}

PlayScene::~PlayScene()
= default;

void PlayScene::draw()
{
	drawDisplayList();
	//Sin = Opposite/Hypotenuse
	//Cos = Adjecent/Hypotenuse
	const float DEG_TO_RADIANS = (double)M_PI / (double)180.0;

	glm::vec2 v = GetLaunchVector(launchAngleDeg, launchSpeed);

	// Starting vector
	SDL_SetRenderDrawColor(Renderer::Instance().getRenderer(), 255, 0, 0, 255);
	SDL_RenderDrawLine(Renderer::Instance().getRenderer(), startPos.x, startPos.y, startPos.x + v.x, startPos.y + v.y);

	{
		// Draw path
		SDL_SetRenderDrawColor(Renderer::Instance().getRenderer(), 0, 0, 255, 255);

		//rearrange to ensure "+ c = 0" part holds
		// at^2 + vt + c = 0
		// at^2 + vt + startPos = groundPosition
		// (1/2 accelerationGravity)t^2 + vt + startPos - groundPosition = 0


		float simTime = 0;
		int NumTimeSteps = 3000;
		glm::vec2 last = startPos;
		glm::vec2 next = startPos;
		for (int step = 0; step < NumTimeSteps; step++)
		{
			next.y = (0.5f * accelerationGravity * massKg) * pow(simTime, 2) + (simTime * v.y) + startPos.y;
			next.x = simTime * v.x + startPos.x;

			SDL_RenderDrawLine(Renderer::Instance().getRenderer(), last.x, last.y, next.x, next.y);
			simTime += 10.0f / NumTimeSteps;
			last = next;
		}
	}

	float c = startPos.y - groundPosition;
	double t1, t2;
	int numSolutions = quadraticFormula(0.5 * accelerationGravity * massKg, v.y, c, t1, t2);

	SDL_Rect targetReticle = SDL_Rect();
	if (numSolutions > 0)
	{
		targetReticle.w = 10;
		targetReticle.h = 10;
		targetReticle.x = t1 * v.x - (targetReticle.w / 2) + startPos.x;
		targetReticle.y = groundPosition - (targetReticle.h / 2);

		SDL_SetRenderDrawColor(Renderer::Instance().getRenderer(), 0, 0, 255, 255);
		SDL_RenderFillRect(Renderer::Instance().getRenderer(), &targetReticle);

		if (numSolutions > 1) // second root
		{
			targetReticle.x = t2 * v.x - (targetReticle.w / 2) + startPos.x;
			SDL_RenderFillRect(Renderer::Instance().getRenderer(), &targetReticle);
		}
	}


	// Floor
	SDL_SetRenderDrawColor(Renderer::Instance().getRenderer(), 0, 0, 0, 255);
	SDL_RenderDrawLine(Renderer::Instance().getRenderer(), -100, groundPosition, 1000, groundPosition);

	// Clear color
	SDL_SetRenderDrawColor(Renderer::Instance().getRenderer(), 255, 255, 255, 255);
}

void PlayScene::update()
{
	updateDisplayList();

	float fn = massKg * accelerationGravity;
	float kf = Util::magnitude({0, fn}) * kFrictionCoefficientFloor;

	float qwe = 0;

	const float dt = 0.0125;//Game::Instance().getDeltaTime();
	m_gameTime += dt;

	for (auto it = projectiles.begin(); it != projectiles.end(); it++)
	{
		Sprite* sprite = *it;

		// ship faces ground
		if (sprite->getRigidBody()->velocity.x <= 0 && sprite->getTransform()->position.y >= groundPosition)
		{
			sprite->getRigidBody()->velocity = {0,0};
			sprite->getRigidBody()->acceleration = {0,0};
		}
		if (sprite->getTransform()->position.y > groundPosition && sprite->getRigidBody()->velocity.x > 0)
		{
			// doesn't drop through ground
			sprite->getRigidBody()->velocity.y = 0;

			sprite->getRigidBody()->acceleration.x = kf * dt;
			sprite->getRigidBody()->velocity.x -= sprite->getRigidBody()->acceleration.x * dt;
			sprite->getTransform()->position += sprite->getRigidBody()->velocity * dt;

			// sprite->getTransform()->rotation.x += sprite->getTransform()->rotation.y * dt;
			
		}
		else
		{
			//sprite->getRigidBody()->velocity.y += accelerationGravity * massKg * dt;
			//sprite->getTransform()->position += sprite->getRigidBody()->velocity * dt;
			
			// sprite->getTransform()->rotation.x += sprite->getTransform()->rotation.y * dt;

			if (sprite->getRigidBody()->velocity.x <= 0)
			{
				sprite->getRigidBody()->velocity.y = 0;

			}
			else
			{
				sprite->getRigidBody()->velocity.y += accelerationGravity * massKg * dt;
				sprite->getTransform()->position += sprite->getRigidBody()->velocity * dt;
			}
		}
		

		std::cout << "Velocity : " << sprite->getRigidBody()->velocity.x << std::endl;
		std::cout << "Velocity : " << sprite->getTransform()->position.x << std::endl;
		
		// setting data into variables
		currentVelocity.x = abs(sprite->getRigidBody()->velocity.x);
		currentVelocity.y = abs(sprite->getRigidBody()->velocity.y);
	}

	for (auto it = projectiles.begin(); it != projectiles.end();)
	{
		Sprite* sprite = *it;
		if (sprite->getTransform()->position.y > 1000)
		{
			it = projectiles.erase(it);
			removeChild(sprite);
		}
		else
		{
			it++;
		}
	}
}

void PlayScene::clean()
{
	removeAllChildren();
}

void PlayScene::handleEvents()
{
	EventManager::Instance().update();

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_UP))
	{
		launchAngleDeg += elevationRate * TheGame::Instance().getDeltaTime();
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_DOWN))
	{
		launchAngleDeg -= elevationRate * TheGame::Instance().getDeltaTime();
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_LEFT))
	{
		launchSpeed -= speedAdjustmentRate * TheGame::Instance().getDeltaTime();
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_RIGHT))
	{
		launchSpeed += speedAdjustmentRate * TheGame::Instance().getDeltaTime();
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_SPACE))
	{
		LaunchProjectile(launchAngleDeg, launchSpeed);
	}

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
	m_gameTime = 0;
	// Set GUI Title
	m_guiTitle = "Play Scene";

	//m_pQuadraticTime = new FuncSprite();
	//addChild(m_pQuadraticTime);

	//m_pQuadraticDelta = new FuncSprite();
	//addChild(m_pQuadraticDelta);
	//m_pQuadraticDelta->getTransform()->position = glm::vec2(c, d);
	//m_pQuadraticDelta->getRigidBody()->acceleration = glm::vec2(0.0f, acceleration);
	//m_pQuadraticDelta->getRigidBody()->velocity = glm::vec2(0.0f, initialVelocity);

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

void PlayScene::LaunchProjectile(float angle, float speed)
{
	Sprite* sprite = new FuncSprite();
	addChild(sprite);
	projectiles.push_back(sprite);
	sprite->getTransform()->position = startPos;
	sprite->getTransform()->rotation.y = rand() % 1000;
	sprite->getRigidBody()->velocity = GetLaunchVector(angle, speed);
}

glm::vec2 PlayScene::GetLaunchVector(float angle, float speed)
{
	const float DEG_TO_RADIANS = (double)M_PI / (double)180.0;
	return glm::vec2(
		cos(-launchAngleDeg * DEG_TO_RADIANS) * launchSpeed,
		sin(-launchAngleDeg * DEG_TO_RADIANS) * launchSpeed
	);
}

void PlayScene::GUI_Function()
{
	// Always open with a NewFrame
	ImGui::NewFrame();

	// See examples by uncommenting the following - also look at imgui_demo.cpp in the IMGUI filter
	//ImGui::ShowDemoWindow();

	ImGui::Begin("Control Panel", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoMove);

	if (ImGui::Button("Launch"))
	{
		LaunchProjectile(launchAngleDeg, launchSpeed);
	}

	ImGui::SliderFloat("angle", &launchAngleDeg, -90.0f, 180.0f, "%.1f,");
	ImGui::SliderFloat("speed", &launchSpeed, -400.0f, 2000.0f, "%.1f,");
	ImGui::SliderFloat2("position", &startPos.x, -0.0f, 900.0f, "%.1f,");
	ImGui::SliderFloat2("velocity", &currentVelocity.x, 0.0f, 0.0f, "%.1f,");
	ImGui::SliderFloat("acceleration", &accelerationGravity, -1000.0f, 1000.0f, "%.1f,");
	ImGui::SliderFloat("ground", &groundPosition, 0.0f, 1200.0f, "%.1f,");
	ImGui::SliderFloat("mass", &massKg, 0.0f, 20.0f, "%.1f,");
	ImGui::SliderFloat("friction", &kFrictionCoefficientFloor, -10.0f, 10.0f, "%.1f,");

	ImGui::End();
}

FuncSprite::FuncSprite()
{
	TextureManager::Instance().load("../Assets/textures/ship3.png", "ship");

	auto size = TextureManager::Instance().getTextureSize("ship");
	setWidth(size.x);
	setHeight(size.y);
}

FuncSprite::~FuncSprite()
{
}

void FuncSprite::draw()
{
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	TextureManager::Instance().draw("ship", x, y, getTransform()->rotation.x, 255, true);
}

void FuncSprite::update()
{
}

void FuncSprite::clean()
{
}
