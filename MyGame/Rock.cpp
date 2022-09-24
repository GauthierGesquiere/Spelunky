#include "pch.h"
#include "Rock.h"

Rock::Rock(const Point2f& center)
	: PickUp(center)
{
	m_pRockTexture = new Texture("Resources\\Images\\Rock.png");

	m_Width = m_pRockTexture->GetWidth();
	m_Height = m_pRockTexture->GetHeight();

	InitSourceRect();
	InitDestinationRect();
}

Rock::~Rock()
{
	delete m_pRockTexture;
	m_pRockTexture = nullptr;
}

void Rock::Update(float elapsedSec, const Level& level, Rectf& player, Vector2f& playerVelocity, const Point2f& hitPoint, bool& isDestroyed, Point2f& pos, bool& isTrowing, bool& isPickedUp, bool& isOtherPickedUp)
{
	PickUp::Update(elapsedSec, level, player, playerVelocity, hitPoint, isDestroyed, pos, isTrowing, isPickedUp, isOtherPickedUp);
}

void Rock::Draw() const
{
	//Debug Drawings
	//utils::DrawRect(m_Shape);

	glPushMatrix();
		glTranslatef(m_Shape.left, m_Shape.bottom, 0);
		glScalef(0.5, 0.5, 0.5);
		m_pRockTexture->Draw(Point2f(0.0f, 0.0f), m_SrcRect);
	glPopMatrix();
}

Rectf Rock::GetPickUpShape() const
{
	return PickUp::GetPickUpShape();
}

Vector2f Rock::GetPickUpVelocity() const
{
	return m_Velocity;
}

