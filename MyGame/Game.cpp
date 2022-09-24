#include "pch.h"
#include "Game.h"
#include <iostream>
#include "utils.h"
#include <fstream>
#include <sstream>

Game::Game( const Window& window ) 
	: m_Window{ window }
	, m_ElapsedSecInvulnerable{ 0 }
	, m_CanTakeDamage{ true }
	, m_CalculateElapsedSecInvelnerable{ 0 }
	, m_EndGame{ false }
{
	Initialize( );
}

Game::~Game( )
{
	Cleanup( );
}

void Game::Initialize()
{
	//Defining HUD
	m_pHUD = new HUD(Point2f{ 20, m_Level.GetBoundaries().height - 200 }, m_Avatar.GetHealth(), m_Window);

	//Defining camera
	m_pCamera = new Camera(m_Window.width, m_Window.height);
	m_pCamera->SetLevelBoundaries(Rectf{ m_Level.GetBoundaries() });

	m_pSoundHit = new SoundEffect("Resources\\Sounds\\Hit.wav");
	m_pPauseInSoundEffect = new SoundEffect("Resources\\Sounds\\Pause_in.wav");
	m_pThemeSong = new SoundStream("Resources\\Sounds\\Spelunky Original (2009) - Full OST Soundtrack.mp3");
	m_pThemeSong->SetVolume(1);
	m_pThemeSong->Play(0);

	//Defining Enemies
	int randNrOfEnemies{ rand() % 5 + 15 };
	int amountOfSpiders{ rand() % (randNrOfEnemies / 4) + 5 };

	for (int i = 0; i < amountOfSpiders; i++)
	{
		GetSpawnPosEnemie("resources\\SpawnPositions.txt", "roof");
	}
	for (int i = 0; i < randNrOfEnemies - amountOfSpiders; i++)
	{
		GetSpawnPosEnemie("resources\\SpawnPositions.txt", "floor");
	}

	//Defining PickUps
	int randNrOfPickUps{ rand() % 5 + 15 };
	for (int i = 0; i < randNrOfPickUps; i++)
	{
		GetSpawnPosPickUp("resources\\SpawnPositions.txt", "floor");
	}
	
}

void Game::Cleanup( )
{
	delete m_pCamera;
	m_pCamera = nullptr;

	delete m_pHUD;
	m_pHUD = nullptr;

	delete m_pSoundHit;
	m_pSoundHit = nullptr;

	delete m_pPauseInSoundEffect;
	m_pPauseInSoundEffect = nullptr;

	delete m_pThemeSong;
	m_pThemeSong = nullptr;

	m_Enemies.~EnemyManager();
	m_PickUps.~PickUpManager();
}

void Game::Update(float elapsedSec)
{
	const Uint8* pKeyStates = SDL_GetKeyboardState(nullptr);

	if (!pKeyStates[SDL_SCANCODE_I])
	{
		CanPressI = true;
	}

	if (pKeyStates[SDL_SCANCODE_I] && CanPressI)
	{
		CanPressI = false;
		std::cout << "--Information--" << std::endl;
		std::cout << "You can move with WASD" << std::endl;
		std::cout << "You can jump with SPACE" << std::endl;
		std::cout << "You can sprint with left SHIFT" << std::endl;
		std::cout << "If you press W or S you look up or down" << std::endl;
		std::cout << "You can whip with K" << std::endl;
		std::cout << "If you press S and J you can pick something up" << std::endl;
		std::cout << "If you press H you can go in the door" << std::endl;
		std::cout << "You can press ESCAPE to go to the menu" << std::endl;
		std::cout << "--EndInformation--" << std::endl;
	}

	if (!pKeyStates[SDL_SCANCODE_ESCAPE])
	{
		m_CanPressEscape = true;
	}


	if (pKeyStates[SDL_SCANCODE_ESCAPE] && m_CanPressEscape && !m_PressedEscape)
	{
		m_CanPressEscape = false;
		m_pPauseInSoundEffect->SetVolume(20);
		m_pPauseInSoundEffect->Play(0);
		m_PressedEscape = true;
	}

	m_pHUD->Update(elapsedSec, m_Avatar.GetIsInDoor(), m_EndGame, m_RestartGame, m_EnemiesKilled, m_PressedEscape);

	if (m_PressedEscape)
		return;

	bool isHit{ false };
	m_Avatar.SetDoorAnimation(m_Level.HasReachedDoor(m_Avatar.GetShape()), m_Level);
	m_Avatar.Update(elapsedSec, m_Level, isHit);

	Rectf player{ m_Avatar.GetShape() };
	Vector2f playerVelocity{ m_Avatar.GetVelocity() };
	bool isDestroyed{ false };
	Point2f pos{ 0,0 };
	bool isTrowing{ false };

	m_Enemies.Update(elapsedSec, m_Level, player, playerVelocity, m_Avatar.GetHitPoint(), isHit, m_EnemiesKilled, m_PickUps.GetPickUps());
	m_PickUps.Update(elapsedSec, m_Level, player, playerVelocity, m_Avatar.GetHitPoint(), isDestroyed, pos, isTrowing);

	if (m_CalculateElapsedSecInvelnerable)
		m_ElapsedSecInvulnerable += elapsedSec;	


	if (isHit)
	{
		m_CalculateElapsedSecInvelnerable = true;
		if (m_CanTakeDamage)
		{		
			m_pSoundHit->SetVolume(15);
			m_pSoundHit->Play(0);
			m_Avatar.GotHit();
			m_pHUD->GotHit();
		}
		m_CanTakeDamage = false;
	}
	if (m_ElapsedSecInvulnerable >= 2.0f)
	{
		m_ElapsedSecInvulnerable = 0.0f;
		m_CanTakeDamage = true;
		m_CalculateElapsedSecInvelnerable = false;
	}
	m_Avatar.SetVelocity(playerVelocity);
	m_Avatar.SetTrowing(isTrowing);

	if (isDestroyed)
	{
		int randNr{ rand() % 5 };
		if (randNr == 0)
			m_Enemies.AddItem(Point2f{ pos.x, pos.y + 1 }, "Snake");
	}
}

void Game::Draw( ) const
{
	ClearBackground( );

	glPushMatrix();
		m_pCamera->Transform(m_Avatar.GetShape(), m_Avatar.GetVerticalDistance());
		DrawLevel();
		m_Enemies.Draw();
		DrawAvatar();
		m_PickUps.Draw();
	glPopMatrix();
	m_pHUD->Draw();
}

void Game::ProcessKeyDownEvent( const SDL_KeyboardEvent & e )
{
	//std::cout << "KEYDOWN event: " << e.keysym.sym << std::endl;
}

void Game::ProcessKeyUpEvent( const SDL_KeyboardEvent& e )
{
	//std::cout << "KEYUP event: " << e.keysym.sym << std::endl;
	//switch ( e.keysym.sym )
	//{
	//case SDLK_LEFT:
	//	//std::cout << "Left arrow key released\n";
	//	break;
	//case SDLK_RIGHT:
	//	//std::cout << "`Right arrow key released\n";
	//	break;
	//case SDLK_1:
	//case SDLK_KP_1:
	//	//std::cout << "Key 1 released\n";
	//	break;
	//}
}

void Game::ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e )
{
	//std::cout << "MOUSEMOTION event: " << e.x << ", " << m_Window.height - e.y << std::endl;
}

void Game::ProcessMouseDownEvent( const SDL_MouseButtonEvent& e )
{
	//std::cout << "MOUSEBUTTONDOWN event: ";
	//switch ( e.button )
	//{
	//case SDL_BUTTON_LEFT:
	//	std::cout << " left button " << std::endl;
	//	break;
	//case SDL_BUTTON_RIGHT:
	//	std::cout << " right button " << std::endl;
	//	break;
	//case SDL_BUTTON_MIDDLE:
	//	std::cout << " middle button " << std::endl;
	//	break;
	//}
}

void Game::ProcessMouseUpEvent( const SDL_MouseButtonEvent& e )
{
	//std::cout << "MOUSEBUTTONUP event: ";
	switch ( e.button )
	{
	case SDL_BUTTON_LEFT:
		m_pHUD->GetMouseMotion(Point2f{ float(e.x), m_Window.height - float(e.y) });
		break;
	case SDL_BUTTON_RIGHT:
		std::cout << " right button " << std::endl;
		break;
	case SDL_BUTTON_MIDDLE:
		std::cout << " middle button " << std::endl;
		break;
	}
}

void Game::ClearBackground( ) const
{
	glClearColor( 0.0f, 0.0f, 0.3f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );
}

bool Game::GameEnd()
{
	return m_EndGame;
}

bool Game::GameRestart()
{
	return m_RestartGame;
}

void Game::DrawAvatar() const
{
	m_Avatar.Draw();
}

void Game::DrawLevel() const
{
	m_Level.DrawBackground();
}

std::string Game::GetAttributeValue(const std::string& attrName, const std::string& element) const
{
	std::string attribute;

	int posFound = int(element.find(attrName + '='));

	int value{ 0 };

	for (unsigned int i = posFound; i < element.size(); i++)
	{
		if (element[i] == '"')
		{
			i++;
			value++;
		}

		if (value == 1)
			attribute += element[i];
		else if (value == 2)
			break;
	}

	return attribute;
}

void Game::GetSpawnPosEnemie(const std::string& fileName, const std::string& place)
{
	std::ifstream File{ fileName };
	std::string text;

	while (getline(File, text, '/'))
	{
		CreateEnemie(text, place);
	}
}

void Game::GetSpawnPosPickUp(const std::string& fileName, const std::string& place)
{
	std::ifstream File{ fileName };
	std::string text;

	while (getline(File, text, '/'))
	{
		CreatePickUp(text, place);
	}
}

void Game::CreateEnemie(const std::string& element, const std::string& place)
{
	if (element.find(place) != std::string::npos && place == "roof")
	{
		SpawnSpider(element);
	}
	else if (element.find(place) != std::string::npos && place == "floor")
	{
		SpawnSnake(element);
	}
}

void Game::CreatePickUp(const std::string& element, const std::string& place)
{
	//25% chance to be a pot
	int isPot{ rand() % 3 };

	if (element.find(place) != std::string::npos && place == "floor")
	{
		if (isPot == 0)		
			SpawnPot(element);	
		else
			SpawnRock(element);
	}
}

Point2f Game::ToPoint2f(const std::string& point2fStr) const
{
	Point2f point;

	std::string x, y;
	int value{ 0 };

	for (unsigned int i = 0; i < point2fStr.size(); i++)
	{
		if (point2fStr[i] == ',')
		{
			i++;
			value++;
		}
		if (value < 1)
			x += point2fStr[i];
		else if (value < 2)
			y += point2fStr[i];
	}
	point.x = std::stof(x);
	point.y = std::stof(y);

	return point;
}

void Game::SpawnSpider(const std::string& element)
{
	std::string pos1;
	std::string pos2;
	int randNr{ rand() % 19 + 1 };
	int posFound = int(element.find(std::to_string(randNr) + "."));
	int value{ 0 };

	for (unsigned int i = posFound; i < element.size(); i++)
	{
		if (element[i] == '"')
		{
			i++;
			value++;
		}

		if (value == 1)
			pos1 += element[i];
		else if (value == 3)
			pos2 += element[i];
		else if (value == 4)
			break;
	}

	Point2f firstPoint{ ToPoint2f(pos1) };
	Point2f lastPoint{ ToPoint2f(pos2) };

	float distance{ lastPoint.x - firstPoint.x };

	Point2f randPos{ rand() % int(distance) + firstPoint.x, firstPoint.y };

	m_Enemies.AddItem(randPos, "Spider");
}

void Game::SpawnSnake(const std::string& element)
{
	std::string pos1;
	std::string pos2;
	int randNr{ rand() % 12 + 1 };
	int posFound = int(element.find(std::to_string(randNr) + "."));
	int value{ 0 };

	for (unsigned int i = posFound; i < element.size(); i++)
	{
		if (element[i] == '"')
		{
			i++;
			value++;
		}

		if (value == 1)
			pos1 += element[i];
		else if (value == 3)
			pos2 += element[i];
		else if (value == 4)
			break;
	}

	Point2f firstPoint{ ToPoint2f(pos1) };
	Point2f lastPoint{ ToPoint2f(pos2) };

	float distance{ lastPoint.x - firstPoint.x };

	Point2f randPos{ rand() % int(distance) + firstPoint.x, firstPoint.y };

	m_Enemies.AddItem(randPos, "Snake");
}

void Game::SpawnRock(const std::string& element)
{
	std::string pos1;
	std::string pos2;
	int randNr{ rand() % 12 + 1 };
	int posFound = int(element.find(std::to_string(randNr) + "."));
	int value{ 0 };

	for (unsigned int i = posFound; i < element.size(); i++)
	{
		if (element[i] == '"')
		{
			i++;
			value++;
		}

		if (value == 1)
			pos1 += element[i];
		else if (value == 3)
			pos2 += element[i];
		else if (value == 4)
			break;
	}

	Point2f firstPoint{ ToPoint2f(pos1) };
	Point2f lastPoint{ ToPoint2f(pos2) };

	float distance{ lastPoint.x - firstPoint.x };

	Point2f randPos{ rand() % int(distance) + firstPoint.x, firstPoint.y };

	m_PickUps.AddItem(randPos, "Rock");
}


void Game::SpawnPot(const std::string& element)
{
	std::string pos1;
	std::string pos2;
	int randNr{ rand() % 12 + 1 };
	int posFound = int(element.find(std::to_string(randNr) + "."));
	int value{ 0 };

	for (unsigned int i = posFound; i < element.size(); i++)
	{
		if (element[i] == '"')
		{
			i++;
			value++;
		}

		if (value == 1)
			pos1 += element[i];
		else if (value == 3)
			pos2 += element[i];
		else if (value == 4)
			break;
	}

	Point2f firstPoint{ ToPoint2f(pos1) };
	Point2f lastPoint{ ToPoint2f(pos2) };

	float distance{ lastPoint.x - firstPoint.x };

	Point2f randPos{ rand() % int(distance) + firstPoint.x, firstPoint.y };

	m_PickUps.AddItem(randPos, "Pot");
}
