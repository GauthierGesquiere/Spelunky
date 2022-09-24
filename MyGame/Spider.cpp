#include "pch.h"
#include "Spider.h"
#include "utils.h"
#include <iostream>

Spider::Spider(const Point2f& center)
	: Enemy(center)
	, m_Jump{ 300 }
	, m_Vision{ 2000 }
	, m_Speed{ 0 }
	, m_IsAbleToMove{ false }
	, m_IsAbleToMoveHorizontaly{ false }
{
	m_Scaler = 0.5f;

	m_LineUnderSpider.push_back(center);
	m_LineUnderSpider.push_back(Point2f{ center.x, center.y - m_Vision });

	m_pSpiderTexture = new Texture("Resources\\Images\\SpiderWalkCycle.png");

	m_SpiderJumpSounEffect = new SoundEffect("Resources\\Sounds\\Spider_Jump.wav");
	m_SpiderlandSoundEffect = new SoundEffect("Resources\\Sounds\\SpiderLand.wav");

	m_Width = m_pSpiderTexture->GetWidth();
	m_Height = m_pSpiderTexture->GetHeight();

	InitSourceRect();
	InitDestinationRect();
	m_Shape.height = 2 * m_Height / 4;
}

Spider::~Spider()
{
	delete m_pSpiderTexture;
	m_pSpiderTexture = nullptr;

	delete m_SpiderJumpSounEffect;
	m_SpiderJumpSounEffect = nullptr;

	delete m_SpiderlandSoundEffect;
	m_SpiderlandSoundEffect = nullptr;

	m_LineUnderSpider.clear();
}

void Spider::Update(const float elapsedSec, const Level& level, Rectf& player, Vector2f& playerVelocity, const Point2f& hitPoint, bool& isHit, bool& isDaed, const std::vector<PickUp*>& pickUps)
{
	utils::HitInfo hitInfo{};
	Enemy::Update(elapsedSec, level, player, playerVelocity, hitPoint, isHit, pickUps);

	if (m_IsAbleToMoveHorizontaly)
		m_ElapsedSecOnGround += elapsedSec;

	if (utils::Raycast(m_LineUnderSpider, Point2f{ player.left - (player.width / 2), player.bottom }, Point2f{ player.left + player.width + (player.width / 2), player.bottom }, hitInfo))
		m_IsAbleToMove = true;

	if (m_IsAbleToMove)
	{
		CalculateDistanceWithplayer(player);
		int volume{};
	    if (DistanceFromPlayer > 1000)
		{
			volume = 2;
		}
		else if (DistanceFromPlayer > 500)
		{
			volume = 7;
		}
		else if (DistanceFromPlayer > 0)
		{
			volume = 15;
		}


		if (!level.IsOnGround(m_Shape, m_Velocity))
		{
			m_CanMakeSound = true;
		}

		if (level.IsOnGround(m_Shape, m_Velocity) && m_CanMakeSound)
		{
			m_CanMakeSound = false;
			m_SpiderlandSoundEffect->SetVolume(volume);
			m_SpiderlandSoundEffect->Play(0);
		}

		if (level.IsOnGround(m_Shape, m_Velocity))
		{
			m_IsAbleToMoveHorizontaly = true;

			if (m_ElapsedSecOnGround >= 2.0f)
			{
				m_SpiderJumpSounEffect->SetVolume(volume);
				m_SpiderJumpSounEffect->Play(0);
				m_Velocity.y = m_Jump;
				m_ElapsedSecOnGround = 0;
				m_Speed = 200;
			}
			else
				m_Speed = 0;

			if (player.left >= m_Shape.left)
				m_Velocity.x = m_Speed;
			else if (player.left < m_Shape.left)
				m_Velocity.x = -m_Speed;
		}

		if (m_IsAbleToMoveHorizontaly)
			m_Shape.left += m_Velocity.x * elapsedSec;

		m_Velocity += m_Acceleration * elapsedSec;
		m_Shape.bottom += m_Velocity.y * elapsedSec;
	}

	isDaed = m_IsKilled;
}

void Spider::Draw() const
{
	//Debug Drawings
	//utils::DrawRect(m_Shape);

	glPushMatrix();
		glTranslatef(m_Shape.left, m_Shape.bottom, 0);
		glTranslatef(m_Shape.width / 2.0f, 0.0f, 0.0f);// half of the with pivot BACK translation
		glScalef(m_Direction * m_Scaler, m_Scaler, m_Scaler);
		if (!m_IsAbleToMove)
		{
			glScalef(1.0f, -1.0f, 1.0f);
			glTranslatef(0.0f, -m_Shape.height * 2, 0.0f);
		}
		glTranslatef(-m_Shape.width * 1.35f, 0.0f, 0.0f); // half of the with pivot translation
		m_pSpiderTexture->Draw(Point2f(0.0f, 0.0f), m_SrcRect);
	glPopMatrix();
}

bool Spider::IsOverlapping(const Rectf& rect) const
{
	return Enemy::IsOverlapping(rect);
}

float Spider::CalculateDistanceWithplayer(const Rectf& player)
{
	if (player.left > m_Shape.left)
	{
		if (player.bottom > m_Shape.bottom)
		{
			DistanceFromPlayer = float(sqrt(((player.left - m_Shape.left) * (player.left - m_Shape.left)) + ((player.bottom - m_Shape.bottom) * (player.bottom - m_Shape.bottom))));
		}
		else if (player.bottom < m_Shape.bottom)
		{
			DistanceFromPlayer = float(sqrt(((player.left - m_Shape.left) * (player.left - m_Shape.left)) + ((m_Shape.bottom - player.bottom) * (m_Shape.bottom - player.bottom))));
		}
	}
	else if (player.left < m_Shape.left)
	{
		if (player.bottom > m_Shape.bottom)
		{
			DistanceFromPlayer = float(sqrt(((m_Shape.left - player.left) * (m_Shape.left - player.left)) + ((player.bottom - m_Shape.bottom) * (player.bottom - m_Shape.bottom))));
		}
		else if (player.bottom < m_Shape.bottom)
		{
			DistanceFromPlayer = float(sqrt(((m_Shape.left - player.left) * (m_Shape.left - player.left)) + ((m_Shape.bottom - player.bottom) * (m_Shape.bottom - player.bottom))));
		}
	}
	return DistanceFromPlayer;
}
