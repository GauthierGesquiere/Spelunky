#pragma once
#include "Avatar.h"
#include "Level.h"
#include "Camera.h"
#include "PickUpManager.h"
#include "HUD.h"
#include "EnemyManager.h"

class Game final
{
public:
	explicit Game( const Window& window );
	Game(const Game& other) = delete;
	Game& operator=(const Game& other) = delete;
	Game( Game&& other) = delete;
	Game& operator=(Game&& other) = delete;
	~Game();

	void Update( float elapsedSec );
	void Draw( ) const;

	// Event handling
	void ProcessKeyDownEvent( const SDL_KeyboardEvent& e );
	void ProcessKeyUpEvent( const SDL_KeyboardEvent& e );
	void ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e );
	void ProcessMouseDownEvent( const SDL_MouseButtonEvent& e );
	void ProcessMouseUpEvent( const SDL_MouseButtonEvent& e );

	bool GameEnd();
	bool GameRestart();

private:
	// DATA MEMBERS
	const Window m_Window;

	Avatar m_Avatar;
	Level m_Level;
	EnemyManager m_Enemies;
	PickUpManager m_PickUps;
	HUD* m_pHUD;
	Camera* m_pCamera;

	SoundEffect* m_pSoundHit;
	SoundEffect* m_pPauseInSoundEffect{};
	SoundStream* m_pThemeSong{};

	bool m_EndGame{};
	bool m_RestartGame{};

	int m_EnemiesKilled{};

	float m_ElapsedSecInvulnerable{  };
	bool m_CanTakeDamage{ };
	bool m_CalculateElapsedSecInvelnerable{  };
	bool m_PressedEscape{};

	bool m_CanPressEscape{};
	bool CanPressI{};

	// FUNCTIONS
	void Initialize( );
	void Cleanup( );
	void ClearBackground( ) const;

	void DrawAvatar() const;
	void DrawLevel() const;

	std::string GetAttributeValue(const std::string& name, const std::string& shape) const;
	void GetSpawnPosEnemie(const std::string& fileName, const std::string& place);
	void GetSpawnPosPickUp(const std::string& fileName, const std::string& place);
	void CreateEnemie(const std::string& shapeElement, const std::string& place);
	void CreatePickUp(const std::string& shapeElement, const std::string& place);
	void SpawnSpider(const std::string& rectangleElement);
	void SpawnSnake(const std::string& rectangleElement);
	void SpawnRock(const std::string& rectangleElement);
	void SpawnPot(const std::string& rectangleElement);
	Point2f ToPoint2f(const std::string& point2fStr) const;
};