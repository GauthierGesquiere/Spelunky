#include "pch.h"
#include "HUD.h"
#include <iostream>

HUD::HUD(const Point2f pos, int TotalHitPoints, Window window)
	: m_BottomLeft{pos}
	, m_TotalHitPoints{ TotalHitPoints }
	, m_Window{window}
	, m_WaitTime{ 2.0f }
	, m_IsDead{ false }
	, m_QuitButtonEndScreen{ 450.0f, 413.0f, 95.0f, 27.0f }
	, m_RestartButtonEndScreen{ 413.0f, 472.0f, 171.0f, 23.0f }
	, m_QuitButtonDeadScreen{ 670.0f, 228.0f, 73.0f, 22.0f }
	, m_RestartButtonDeadScreen{ 636.0f , 260.0f, 136.0f, 21.0f }
	, m_TopRightTimerPos{ 800, 750 }
	, m_ResumeButtonMenuScreen{ 419, 408, 165, 18}
	, m_RestartButtonMenuScreen{ 452, 366, 89, 15}
	, m_QuitButtonMenuScreen{ 436, 320, 105, 20 }
{
	m_pHeartTexture = new Texture("Resources\\Images\\Heart.png");
	m_pNumberTexture = new Texture("Resources\\Images\\" + std::to_string(m_TotalHitPoints) + "HP_Transparant.png");

	m_pEndScreenTexture = new Texture("Resources\\Images\\EndScreen.png");
	m_pDeadScreenTexture = new Texture("Resources\\Images\\DeadScreen.png");
	m_pMenuScreenTexture = new Texture("Resources\\Images\\MenuScreen.png");

	m_pTimerTexture = new Texture("0:0:0", "Resources\\Fonts\\Tekton-Bold.otf", 30, Color4f{ 1,1,1,1 });
	m_pBackGroundTimerTexture = new Texture("Resources\\Images\\BackGroundTimer.png");
	m_pKillsTexture = new Texture("0", "Resources\\Fonts\\Tekton-Bold.otf", 30, Color4f{ 1,1,1,1 });
	
	m_SrcRectHeart = Rectf{ 0, 0, m_pHeartTexture->GetWidth(), m_pHeartTexture->GetHeight() };
	m_SrcRectNumber = Rectf{ 0, 0, m_pNumberTexture->GetWidth(), m_pNumberTexture->GetHeight() };
	m_SrcRectEndScreen = Rectf{ 0, 0, m_pEndScreenTexture->GetWidth(), m_pEndScreenTexture->GetHeight() };
	m_SrcRectDeadScreen = Rectf{ 0, 0, m_pDeadScreenTexture->GetWidth(), m_pDeadScreenTexture->GetHeight() };
	m_SrcRectTimer = Rectf{ 0, 0, m_pTimerTexture->GetWidth(), m_pTimerTexture->GetHeight() };
	m_SrcRectBackGroundTimer = Rectf{ 0, 0, m_pBackGroundTimerTexture->GetWidth(), m_pBackGroundTimerTexture->GetHeight() };
	m_SrcRectEnemiesKilled = Rectf{ 0, 0, m_pKillsTexture->GetWidth(), m_pKillsTexture->GetHeight() };
	m_SrcRectMenuScreen = Rectf{ 0, 0, m_pMenuScreenTexture->GetWidth(), m_pMenuScreenTexture->GetHeight() };

	m_pPauseOutSoundEffect = new SoundEffect("Resources\\Sounds\\Pause_out.wav");
	m_pEndSoundEffect = new SoundEffect("Resources\\Sounds\\lobbyDrum.wav");
	m_pWalksInDoorSoundEffect = new SoundEffect("Resources\\Sounds\\InToDoor.wav");
	m_pDeathSoundEffect = new SoundEffect("Resources\\Sounds\\Death.wav");

	numberPos = Point2f{ m_BottomLeft.x + (m_SrcRectHeart.width / 2) - m_pNumberTexture->GetWidth() / 2, m_BottomLeft.y + (m_SrcRectHeart.height / 2) - m_pNumberTexture->GetHeight() / 2 };
}

HUD::~HUD()
{
	delete m_pHeartTexture;
	m_pHeartTexture = nullptr;

	delete m_pNumberTexture;
	m_pNumberTexture = nullptr;

	delete m_pEndScreenTexture;
	m_pEndScreenTexture = nullptr;

	delete m_pDeadScreenTexture;
	m_pDeadScreenTexture = nullptr;

	delete m_pTimerTexture;
	m_pTimerTexture = nullptr;

	delete m_pKillsTexture;
	m_pKillsTexture = nullptr;

	delete m_pBackGroundTimerTexture;
	m_pBackGroundTimerTexture = nullptr;

	delete m_pMenuScreenTexture;
	m_pMenuScreenTexture = nullptr;

	delete m_pPauseOutSoundEffect;
	m_pPauseOutSoundEffect = nullptr;

	delete m_pEndSoundEffect;
	m_pEndSoundEffect = nullptr;

	delete m_pWalksInDoorSoundEffect;
	m_pWalksInDoorSoundEffect = nullptr;

	delete m_pDeathSoundEffect;
	m_pDeathSoundEffect = nullptr;
}


void HUD::Update(float elapsedSec, bool isInDoor, bool& endGame, bool& restartGame, int enemiesKilled, bool& pressedEscape)
{
	m_PressedEscape = pressedEscape;

	if (!m_IsDead)
		m_EnemiesKilled = enemiesKilled;

	if (!m_IsDead && !m_ReachedTheEnd && !m_PressedEscape)
		m_OneSec += elapsedSec;

	if (m_OneSec > 1)
	{
		delete m_pTimerTexture;
		m_TimerSec++;
		m_pTimerTexture = new Texture(std::to_string(m_TimerHours) + ":" + std::to_string(m_TimerMin) + ":" + std::to_string(m_TimerSec), "Resources\\Fonts\\Tekton-Bold.otf", 30, Color4f{ 1,1,1,1 });
		m_SrcRectTimer = Rectf{ 0, 0, m_pTimerTexture->GetWidth(), m_pTimerTexture->GetHeight() };
		m_OneSec = 0;
	}
	if (m_TimerSec >= 59)
	{
		m_TimerMin++;
		m_TimerSec = 0;
	}
	if (m_TimerMin >= 59)
	{
		m_TimerHours++;
		m_TimerMin = 0;
	}

	if (!m_IsDead)
	{
		m_CanPlayDeathSound = true;
	}
	if (m_IsDead && m_CanPlayDeathSound)
	{
		m_CanPlayDeathSound = false;
		m_pDeathSoundEffect->SetVolume(15);
		m_pDeathSoundEffect->Play(0);
	}


	if (m_IsDead)
	{
		if (utils::IsPointInRect(m_MousePos, m_QuitButtonDeadScreen))
		{
			endGame = true;
			return;
		}

		if (utils::IsPointInRect(m_MousePos, m_RestartButtonDeadScreen))
		{
			restartGame = true;
			return;
		}
		m_ElapsedSecDiedScreen += elapsedSec;
	}

	if (m_PressedEscape)
	{
		if (utils::IsPointInRect(m_MousePos, m_ResumeButtonMenuScreen))
		{
			m_pPauseOutSoundEffect->SetVolume(20);
			m_pPauseOutSoundEffect->Play(0);
			pressedEscape = false;
			m_MenuIsOpen = false;
			m_MousePos.x = 0.0f;
		}

		if (utils::IsPointInRect(m_MousePos, m_RestartButtonMenuScreen))
		{
			restartGame = true;
			return;
		}

		if (utils::IsPointInRect(m_MousePos, m_QuitButtonMenuScreen))
		{
			endGame = true;
			return;
		}
	}


	m_ReachedTheEnd = isInDoor;

	if (!m_ReachedTheEnd)
	{
		m_CanPlayEndSound = true;
		m_CanPlayWalkInDoorSound = true;
	}	
	
	if (m_ReachedTheEnd && m_CanPlayEndSound)
	{
		if (m_CanPlayWalkInDoorSound)
		{
			m_pWalksInDoorSoundEffect->SetVolume(20);
			m_pWalksInDoorSoundEffect->Play(0);
			m_CanPlayWalkInDoorSound = false;
		}
		if (m_ElapsedSecEndScreen >= m_WaitTime)
		{
			m_CanPlayEndSound = false;
			m_pEndSoundEffect->SetVolume(20);
			m_pEndSoundEffect->Play(0);
		}
	}


	if (m_ReachedTheEnd)
	{
		if (utils::IsPointInRect(m_MousePos, m_QuitButtonEndScreen))
		{
			endGame = true;
			return;
		}
		if (utils::IsPointInRect(m_MousePos, m_RestartButtonEndScreen))
		{
			restartGame = true;
			return;
		}
		m_ElapsedSecEndScreen += elapsedSec;
	}
}

void HUD::Draw() const
{
	glPushMatrix();
		glScalef(0.5f, 0.5f, 1);
		m_pBackGroundTimerTexture->Draw(Point2f{ 2 * (m_TopRightTimerPos.x - m_pBackGroundTimerTexture->GetWidth() / 4) ,  2 * (m_TopRightTimerPos.y - m_pBackGroundTimerTexture->GetHeight() / 4) }, m_SrcRectBackGroundTimer);
		glScalef(2.f, 2.f, 1);
		m_pTimerTexture->Draw(Point2f{ m_TopRightTimerPos.x - m_pTimerTexture->GetWidth() / 2, m_TopRightTimerPos.y - 20 }, m_SrcRectTimer);

		if (m_TotalHitPoints == 0)
			ShowDeadScreen();

		if (m_ReachedTheEnd)
			ShowEndScreen();

		if (m_PressedEscape)
			ShowMenuScreen();


		glScalef(0.4f, 0.4f, 0.4f);
		if (!m_EndScreenIsOnScreen)
		{
			m_pHeartTexture->Draw(m_BottomLeft, m_SrcRectHeart);
			m_pNumberTexture->Draw(numberPos , m_SrcRectNumber);
		}

		
	glPopMatrix();
}

void HUD::GotHit()
{
	if (m_TotalHitPoints == 0)
		return;

	m_TotalHitPoints--;

	delete m_pNumberTexture;
	m_pNumberTexture = nullptr;

	if (m_TotalHitPoints >= 0)
		m_pNumberTexture = new Texture("Resources\\Images\\" + std::to_string(m_TotalHitPoints) + "HP_Transparant.png");
	
}

void HUD::GetMouseMotion(Point2f point)
{
	m_MousePos = point;
}

void HUD::ShowEndScreen() const
{
	glPushMatrix();
	if (m_ElapsedSecEndScreen >= m_WaitTime)
	{
		Point2f timerOffset{ 0, 20 };
		m_EndScreenIsOnScreen = true;
		m_pEndScreenTexture->Draw(Point2f{ 0.0f, 0.0f }, m_SrcRectEndScreen);
		m_pTimerTexture->Draw(Point2f{ m_Window.width / 2 - m_pTimerTexture->GetWidth() / 2, m_Window.height - m_Window.height / 4 + timerOffset .y}, m_SrcRectTimer);
	}
	glPopMatrix();
}

void HUD::ShowDeadScreen() const
{
	m_IsDead = true;

	delete m_pKillsTexture;
	m_pKillsTexture = new Texture(std::to_string(m_EnemiesKilled), "Resources\\Fonts\\Tekton-Bold.otf", 30, Color4f{ 1,1,1,1 });
	m_SrcRectEnemiesKilled = Rectf{ 0, 0, m_pKillsTexture->GetWidth(), m_pKillsTexture->GetHeight() };

	glPushMatrix();
	if (m_ElapsedSecDiedScreen >= m_WaitTime)
	{
		Point2f timerOffset{ 10, 25 };
		Point2f KillsOffset{ 20, 65 };
		m_pDeadScreenTexture->Draw(Point2f{ (m_Window.width / 2) - (m_pDeadScreenTexture->GetWidth() / 2), (m_Window.height / 2) - (m_pDeadScreenTexture->GetHeight() / 2) }, m_SrcRectDeadScreen);
		m_pTimerTexture->Draw(Point2f{ m_Window.width / 2 + m_Window.width / 4 + timerOffset.x,  m_Window.height / 2 + timerOffset.y }, m_SrcRectTimer);
		m_pKillsTexture->Draw(Point2f{ m_Window.width / 2 + m_Window.width / 4 - KillsOffset.x,  m_Window.height / 2 + KillsOffset.y }, m_SrcRectEnemiesKilled);
	}
	glPopMatrix();
}

void HUD::ShowMenuScreen() const
{
	m_MenuIsOpen = true;
	m_pMenuScreenTexture->Draw(Point2f{m_Window.width / 2 - m_SrcRectMenuScreen.width / 2, m_Window.height / 2 - m_SrcRectMenuScreen.height / 2}, m_SrcRectMenuScreen);

}
