#include "pch.h"
#include "Pot.h"

Pot::Pot(const Point2f& center)
	: m_TotalAmountOfPots{ 3 }
	, PickUp(center)
{
	m_Scaler = 3.0f;

	int randNr{ rand() % m_TotalAmountOfPots + 1 };
	std::string EnemieName{ "Pot" + std::to_string(randNr) };

	//hitbox
	/*m_Shape.height = 30;
	m_Shape.width = 45;*/

	m_pPotTexture = new Texture("Resources\\Images\\" + EnemieName + ".png");
	m_Width = m_pPotTexture->GetWidth();
	m_Height = m_pPotTexture->GetHeight();

	InitSourceRect();
	InitDestinationRect();

}

Pot::~Pot()
{
	delete m_pPotTexture;
	m_pPotTexture = nullptr;
}

void Pot::Update(float elapsedSec, const Level& level, Rectf& player, Vector2f& playerVelocity, const Point2f& hitPoint, bool& isDestroyed, Point2f& pos, bool& isTrowing, bool& isPickedUp, bool& isOtherPickedUp)
{
	utils::HitInfo hitInfo{};

	PickUp::Update(elapsedSec, level, player, playerVelocity, hitPoint, isDestroyed, pos, isTrowing, isPickedUp, isOtherPickedUp);

	std::vector<Point2f> vectorWhip;
	vectorWhip.push_back(Point2f{ player.left + player.width / 2, player.bottom + player.height / 2 });
	vectorWhip.push_back(Point2f{ hitPoint.x, hitPoint.y });

	if (hitPoint.x != 0.0f)
	{
		if (utils::Raycast(vectorWhip, Point2f{ m_Shape.left, m_Shape.bottom }, Point2f{ m_Shape.left , m_Shape.bottom + m_Shape.height }, hitInfo))
		{
			m_IsDestroyed = true;
		}
		if (utils::Raycast(vectorWhip, Point2f{ m_Shape.left, m_Shape.bottom + m_Shape.height }, Point2f{ m_Shape.left + m_Shape.width, m_Shape.bottom + m_Shape.height }, hitInfo))
		{
			m_IsDestroyed = true;
		}
		if (utils::Raycast(vectorWhip, Point2f{ m_Shape.left + m_Shape.width, m_Shape.bottom + m_Shape.height }, Point2f{ m_Shape.left + m_Shape.width, m_Shape.bottom }, hitInfo))
		{
			m_IsDestroyed = true;
		}
		if (utils::Raycast(vectorWhip, Point2f{ m_Shape.left + m_Shape.width, m_Shape.bottom }, Point2f{ m_Shape.left , m_Shape.bottom }, hitInfo))
		{
			m_IsDestroyed = true;
		}
	}

	if (m_IsDestroyed)
		pos = GetPosition();

	isDestroyed = m_IsDestroyed;
}

void Pot::Draw() const
{
	//Debug Drawings
	//utils::DrawRect(m_Shape);

	glPushMatrix();
		glTranslatef(m_Shape.left, m_Shape.bottom, 0);
		glScalef(0.5, 0.5, 0.5);
		m_pPotTexture->Draw(Point2f(0.0f, 0.0f), m_SrcRect);
	glPopMatrix();
}

Rectf Pot::GetPickUpShape() const
{
	return PickUp::GetPickUpShape();
}

Vector2f Pot::GetPickUpVelocity() const
{
	return m_Velocity;
}

