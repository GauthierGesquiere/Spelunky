#pragma once
#include "Texture.h"
#include "utils.h"
#include "SoundEffect.h"

class HUD
{
public: 
	explicit HUD(const Point2f pos, int TotalHitPoints, Window window);
	HUD(const HUD& other) = delete;
	HUD& operator=(const HUD& other) = delete;
	HUD(HUD&& other) = delete;
	HUD& operator=(HUD&& other) = delete;
	~HUD();

	void Update(float elapsedSec, bool isInDoor, bool& endGame, bool& restartGame, int enemiesKilled, bool& pressedEscape);
	void Draw() const;
	void GotHit();

	void GetMouseMotion(Point2f point);

	void ShowEndScreen() const;
	void ShowDeadScreen() const;
	void ShowMenuScreen() const;

private:
	Window m_Window{};

	SoundEffect* m_pPauseOutSoundEffect{};
	SoundEffect* m_pEndSoundEffect{};
	SoundEffect* m_pWalksInDoorSoundEffect{};
	SoundEffect* m_pDeathSoundEffect{};

	mutable float m_ElapsedSecDiedScreen{};
	mutable float m_ElapsedSecEndScreen{};
	float m_WaitTime{  };
	float m_OneSec{};

	mutable bool m_IsDead{};
	bool m_ReachedTheEnd{};
	bool m_RestartGame{};
	bool m_CanPlayEndSound{};
	bool m_CanPlayWalkInDoorSound{};
	bool m_CanPlayDeathSound{};

	bool m_PressedEscape{};
	mutable bool m_MenuIsOpen{};

	mutable bool m_EndScreenIsOnScreen{};

	int m_TimerSec{};
	int m_TimerMin{};
	int m_TimerHours{};

	int m_EnemiesKilled{};

	Point2f m_MousePos{};
	Rectf m_QuitButtonEndScreen{};
	Rectf m_RestartButtonEndScreen{};
	Rectf m_QuitButtonDeadScreen{};
	Rectf m_RestartButtonDeadScreen{};
	Rectf m_ResumeButtonMenuScreen{};
	Rectf m_RestartButtonMenuScreen{};
	Rectf m_QuitButtonMenuScreen{};
	Point2f m_TopRightTimerPos{};

	Point2f m_BottomLeft{};
	int m_TotalHitPoints{};
	Texture* m_pHeartTexture;
	Texture* m_pNumberTexture;

	Texture* m_pTimerTexture;
	Texture* m_pBackGroundTimerTexture;
	mutable Texture* m_pKillsTexture;

	Texture* m_pEndScreenTexture;
	Texture* m_pDeadScreenTexture;
	Texture* m_pMenuScreenTexture;

	Rectf m_SrcRectHeart{};
	Rectf m_SrcRectNumber{};
	Rectf m_SrcRectDeadScreen{};
	Rectf m_SrcRectEndScreen{};
	Rectf m_SrcRectTimer{};
	mutable Rectf m_SrcRectEnemiesKilled{};
	Rectf m_SrcRectBackGroundTimer{};
	Rectf m_SrcRectMenuScreen{};

	Point2f numberPos{};
};

