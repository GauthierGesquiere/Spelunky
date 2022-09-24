#include "pch.h"
#include "Enemies.h"
#include "utils.h"
#include <iostream>
#include "Game.h"


Enemies::Enemies(const Point2f& center, Enemies::TypeEnemie sortEnemie)
	: m_Enemie{sortEnemie}
	, m_StartPos{center}
	, m_ClipHeight{67.0f}
	, m_ClipWidth{ 80.0f }
	, m_NrOfFrames{ 11 }
	, m_FramesPerSec{ 10 }
	, m_Direction{ 1 }
	, m_ElapsedSecOnGround{ 0 } 
	, m_InvulnerabilitySec{ 0.2f }
	, m_Velocity{ 0.0f, 0.0f }
	, m_Spiderjump{ 300 }
	, m_SpiderSpeed{ 0 }
	, m_Acceleration{ 0, -981.0f }
	, m_Scaler{ 1.0f }
	, m_Bounce{ 200 }
	, m_SpiderVision{ 2000 }
	, m_IsAbleToMove{ false }
	, m_IsAbleToMoveHorizontaly{ false }
	, m_IsKilled{ false }

{
	std::string EnemieName;
	if (TypeEnemie::Snake == m_Enemie)
	{
		EnemieName = "Snake";
	}
	else if (TypeEnemie::Spider == m_Enemie)
	{
		EnemieName = "Spider";
		m_Scaler = 0.5f;
		m_Vertices.push_back(center);
		m_Vertices.push_back(Point2f{ center.x, center.y - m_SpiderVision }); 
	}
	m_pTextureToDraw = new Texture("Resources\\Images\\" + EnemieName + "WalkCycle.png");

	InitSourceRect();
	InitDestinationRect();
}

Enemies::~Enemies()
{
	delete m_pTextureToDraw;
	m_pTextureToDraw = nullptr;
}

void Enemies::Update(float elapsedSec, const Level& level, Rectf& player, Vector2f& playerVelocity, const Point2f& hitPoint, bool& isHit, bool& isDaed)
{
	m_InvulnerabilitySec -= elapsedSec;

	UpdateTransforming(elapsedSec);
	level.HandleCollision(m_Shape, m_Velocity, false);

	if (m_IsKilled)
		Enemies::~Enemies();


	if (m_IsAbleToMoveHorizontaly)
		m_ElapsedSecOnGround += elapsedSec;


	//Looks if Hit
	//Can be better
	if (utils::IsPointInRect(hitPoint, m_Shape))
		if (m_InvulnerabilitySec <= 0.0f)
			m_IsKilled = true;


	
	if ((m_Shape.left < (player.left + (player.width / 2))) && (player.left + (player.width / 2) < m_Shape.left + m_Shape.width))
	{
		//IfOnTopOfEnemie;
		if (playerVelocity.y < 0)
		{
			if (((m_Shape.bottom + m_Shape.height) - player.bottom) < 5.f && ((m_Shape.bottom + m_Shape.height) - player.bottom) > 0.f)
			{
				m_IsKilled = true;
				playerVelocity.y = m_Bounce;
			}
		}
			
		//IsInEnemie
		if ((m_Shape.bottom < (player.bottom + (player.height / 2))) && (player.bottom + (player.height / 2) < m_Shape.bottom + m_Shape.height))
		{
			playerVelocity.y = m_Bounce;
			isHit = true;
			m_IsAbleToMove = true;
		}		
	}
	
	
	utils::HitInfo hitInfo{};

	switch (m_Enemie)
	{
	case Enemies::TypeEnemie::Spider:

		if (utils::Raycast(m_Vertices, Point2f{ player.left - (player.width / 2), player.bottom }, Point2f{ player.left + player.width + (player.width / 2), player.bottom }, hitInfo))
			m_IsAbleToMove = true;

		if (m_IsAbleToMove)
		{
			if (level.IsOnGround(m_Shape, m_Velocity))
			{
				m_IsAbleToMoveHorizontaly = true;

				if (m_ElapsedSecOnGround >= 2.0f)
				{
					m_Velocity.y = m_Spiderjump;
					m_ElapsedSecOnGround = 0;
					m_SpiderSpeed = 200;
				}
				else 
					m_SpiderSpeed = 0;

				if (player.left >= m_Shape.left)
					m_Velocity.x = m_SpiderSpeed;
				else if (player.left < m_Shape.left)
					m_Velocity.x = -m_SpiderSpeed;

			} 
			
			if (m_IsAbleToMoveHorizontaly)
				m_Shape.left += m_Velocity.x * elapsedSec;
			
			m_Velocity += m_Acceleration * elapsedSec;
			m_Shape.bottom += m_Velocity.y * elapsedSec;
		}
		break;

	case Enemies::TypeEnemie::Snake:

		m_Velocity.x = 50;

		m_Velocity += m_Acceleration * elapsedSec;
		m_Velocity.x *= m_Direction;
		m_Shape.left += m_Velocity.x * elapsedSec;
		m_Shape.bottom += m_Velocity.y * elapsedSec;

		if (level.IsOnledge(m_Shape, m_Velocity) || (m_Velocity.x == 0))
			m_Direction = -m_Direction;
		break;

	default:
		break;
	}

	isDaed = m_IsKilled;
}

void Enemies::Draw() const
{
	//Debug Drawings
	//utils::DrawRect(m_Shape);
	
	glPushMatrix();
		glTranslatef(m_Shape.left, m_Shape.bottom, 0);
		glTranslatef(m_Shape.width / 2.0f, 0.0f, 0.0f);// half of the with pivot BACK translation
		glScalef(m_Direction * m_Scaler, m_Scaler, m_Scaler);
		if (m_Enemie == TypeEnemie::Spider)
		{
			if (!m_IsAbleToMove)
			{
				glScalef(1.0f, -1.0f, 1.0f);
				glTranslatef(0.0f, -m_Shape.height * 2, 0.0f);
			}
		}
		glTranslatef(-m_Shape.width * 1.35f, 0.0f, 0.0f); // half of the with pivot translation
		m_pTextureToDraw->Draw(Point2f(0.0f, 0.0f), m_SrcRect);
	glPopMatrix();
	
}

bool Enemies::IsOverlapping(const Rectf& rect) const
{
	return utils::IsOverlapping(rect, m_Shape);
}

void Enemies::UpdateTransforming(float elapsedSec)
{
	if (m_Enemie == TypeEnemie::Snake)
	{
		m_CurrentTime += elapsedSec;
		if (m_CurrentTime > (1.f / m_FramesPerSec))
		{
			++m_CurrentFrame %= m_NrOfFrames;
			m_CurrentTime -= (1.f / m_FramesPerSec);
			UpdateSourceRect();
		}
	}
}

void Enemies::InitDestinationRect()
{
	//Hitbox
	m_Shape.width = 1.5f * m_ClipWidth / 4;
	m_Shape.height = 2 * m_ClipHeight / 4;
	if (m_Enemie == TypeEnemie::Snake)
		m_Shape.height = 3 * m_ClipHeight / 4;

	//Startpoint
	m_Shape.left = m_StartPos.x;
	m_Shape.bottom = m_StartPos.y;
}

void Enemies::InitSourceRect()
{
	m_SrcRect.width = m_ClipWidth;
	m_SrcRect.height = m_ClipHeight;
	m_SrcRect.bottom = 0;
	UpdateSourceRect();
}

void Enemies::UpdateSourceRect()
{
	m_SrcRect.left = m_CurrentFrame % m_NrOfFrames * m_SrcRect.width;
}


