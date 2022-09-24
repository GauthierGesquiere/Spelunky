#include "pch.h"
#include "Snake.h"
#include "Texture.h"
#include "Level.h"
#include "utils.h"

Snake::Snake(const Point2f& center)
	: Enemy(center)
	, m_NrOfFrames{ 11 }
	, m_FramesPerSec{ 10 }
{

	m_pSnakeTexture = new Texture("Resources\\Images\\SnakeWalkCycle.png");

	m_Width = m_pSnakeTexture->GetWidth() / m_NrOfFrames;
	m_Height = m_pSnakeTexture->GetHeight();

	InitSourceRect();
	InitDestinationRect();
	m_Shape.height = 3 * m_Height / 4;
}

Snake::~Snake()
{
	delete m_pSnakeTexture;
	m_pSnakeTexture = nullptr;
}

void Snake::Update(const float elapsedSec, const Level& level, Rectf& player, Vector2f& playerVelocity, const Point2f& hitPoint, bool& isHit, bool& isDaed, const std::vector<PickUp*>& pickUps)
{
	Enemy::Update(elapsedSec, level, player, playerVelocity, hitPoint, isHit, pickUps);

	UpdateTransforming(elapsedSec);

	m_Velocity.x = 50;

	m_Velocity += m_Acceleration * elapsedSec;
	m_Velocity.x *= m_Direction;
	m_Shape.left += m_Velocity.x * elapsedSec;
	m_Shape.bottom += m_Velocity.y * elapsedSec;

	if (level.IsOnledge(m_Shape, m_Velocity) || (m_Velocity.x == 0))
		m_Direction = -m_Direction;

	isDaed = m_IsKilled;
}

void Snake::Draw() const
{
	//Debug Drawings
	//utils::DrawRect(m_Shape);

	glPushMatrix();
		glTranslatef(m_Shape.left, m_Shape.bottom, 0);
		glTranslatef(m_Shape.width / 2.0f, 0.0f, 0.0f);// half of the with pivot BACK translation
		glScalef(m_Direction * m_Scaler, m_Scaler, m_Scaler);
		glTranslatef(-m_Shape.width * 1.35f, 0.0f, 0.0f); // half of the with pivot translation
		m_pSnakeTexture->Draw(Point2f(0.0f, 0.0f), m_SrcRect);
	glPopMatrix();
}

bool Snake::IsOverlapping(const Rectf& rect) const
{
	return Enemy::IsOverlapping(rect);
}

void Snake::UpdateTransforming(float elapsedSec)
{
	m_CurrentTime += elapsedSec;
	if (m_CurrentTime > (1.f / m_FramesPerSec))
	{
		++m_CurrentFrame %= m_NrOfFrames;
		m_CurrentTime -= (1.f / m_FramesPerSec);
		UpdateSourceRect();
	}
}

void Snake::UpdateSourceRect()
{
	m_SrcRect.left = m_CurrentFrame % m_NrOfFrames * m_SrcRect.width;
}
