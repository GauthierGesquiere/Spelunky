#include "pch.h"
#include "Enemy.h"
#include "utils.h"
#include "Rock.h"
#include "Pot.h"
#include <typeinfo>

Enemy::Enemy(const Point2f& center)
	: m_Center{ center }
	, m_Width{ 80.0f }
	, m_Height{ 67.0f }
	, m_Direction{ 1 }
	, m_ElapsedSecOnGround{ 0 }
	, m_InvulnerabilitySec{ 0.2f }
	, m_Velocity{ 0.0f, 0.0f }
	, m_Acceleration{ 0, -981.0f }
	, m_Scaler{ 1.0f }
	, m_Bounce{ 300 }
	, m_IsKilled{ false }
{
	InitSourceRect();
	InitDestinationRect();


}

Enemy::~Enemy()
{
}

void Enemy::ChecksIsOnTopOfEnemy(Rectf& player, Vector2f& playerVelocity)
{
	utils::HitInfo hitInfo{};

	std::vector<Point2f> vectorMiddleOfPlayer;
	vectorMiddleOfPlayer.push_back(Point2f{ player.left + player.width / 2, player.bottom + player.height / 2 });
	vectorMiddleOfPlayer.push_back(Point2f{ player.left + player.width / 2, player.bottom });

	if (utils::Raycast(vectorMiddleOfPlayer, Point2f{ m_Shape.left, m_Shape.bottom + m_Shape.height }, Point2f{ m_Shape.left + m_Shape.width, m_Shape.bottom + m_Shape.height }, hitInfo))
	{
		if (playerVelocity.y < 0.0f)
		{
			m_IsKilled = true;
			playerVelocity.y = m_Bounce;
		}
	}
}

void Enemy::ChecksIsPickUpHittingEnemy(const std::vector<PickUp*>& pickUps, const Level& level)
{
	Rectf pickUpShape{};
	Vector2f pickUpVelocity{};
	for (PickUp* ptr : pickUps)
	{
		if (typeid(*ptr) == typeid(Rock))
		{
			Rock* pRock{ static_cast<Rock*>(ptr) };
			pickUpShape = pRock->GetPickUpShape();
			pickUpVelocity = pRock->GetPickUpVelocity();
		}
		else if (typeid(*ptr) == typeid(Pot))
		{
			Pot* pPot{ static_cast<Pot*>(ptr) };
			pickUpShape = pPot->GetPickUpShape();
			pickUpVelocity = pPot->GetPickUpVelocity();
		}

		if (pickUpVelocity.x != 0)
		{
			if ((m_Shape.left < (pickUpShape.left + (pickUpShape.width / 2))) && (pickUpShape.left + (pickUpShape.width / 2) < m_Shape.left + m_Shape.width))
			{
				if ((m_Shape.bottom < (pickUpShape.bottom + (pickUpShape.height / 2))) && (pickUpShape.bottom + (pickUpShape.height / 2) < m_Shape.bottom + m_Shape.height))
				{
					m_IsKilled = true;
				}
			}
		}

	}
}

void Enemy::ChecksIsInEnemy(Rectf& player, Vector2f& playerVelocity, bool& isHit)
{
	if ((m_Shape.left < (player.left + (player.width / 2))) && (player.left + (player.width / 2) < m_Shape.left + m_Shape.width))
	{
		if ((m_Shape.bottom < (player.bottom + (player.height / 2))) && (player.bottom + (player.height / 2) < m_Shape.bottom + m_Shape.height))
		{
			playerVelocity.y = m_Bounce;
			isHit = true;
			//m_IsAbleToMove = true;
		}
	}
}

void Enemy::Update(const float elapsedSec, const Level& level, Rectf& player, Vector2f& playerVelocity, const Point2f& hitPoint, bool& isHit, const std::vector<PickUp*>& pickUps)
{
	utils::HitInfo hitInfo{};

	m_InvulnerabilitySec -= elapsedSec;

	std::vector<Point2f> vectorWhip;
	vectorWhip.push_back(Point2f{ player.left + player.width / 2, player.bottom + player.height / 2 });
	vectorWhip.push_back(Point2f{ hitPoint.x, hitPoint.y });

	if (m_InvulnerabilitySec <= 0.0f && hitPoint.x != 0.0f)
	{
		if (utils::Raycast(vectorWhip, Point2f{ m_Shape.left, m_Shape.bottom }, Point2f{ m_Shape.left , m_Shape.bottom + m_Shape.height }, hitInfo))
		{
			m_IsKilled = true;
		}
		if (utils::Raycast(vectorWhip, Point2f{ m_Shape.left, m_Shape.bottom + m_Shape.height }, Point2f{ m_Shape.left + m_Shape.width, m_Shape.bottom + m_Shape.height }, hitInfo))
		{
			m_IsKilled = true;
		}
		if (utils::Raycast(vectorWhip, Point2f{ m_Shape.left + m_Shape.width, m_Shape.bottom + m_Shape.height }, Point2f{ m_Shape.left + m_Shape.width, m_Shape.bottom }, hitInfo))
		{
			m_IsKilled = true;
		}
		if (utils::Raycast(vectorWhip, Point2f{ m_Shape.left + m_Shape.width, m_Shape.bottom }, Point2f{ m_Shape.left , m_Shape.bottom }, hitInfo))
		{
			m_IsKilled = true;
		}
	}
	
	/*if (utils::IsPointInRect(hitPoint, m_Shape))
		if (m_InvulnerabilitySec <= 0.0f)
			m_IsKilled = true;*/

	level.HandleCollision(m_Shape, m_Velocity, false);

	if (m_IsKilled)
		Enemy::~Enemy();

	ChecksIsOnTopOfEnemy(player, playerVelocity);
	ChecksIsInEnemy(player, playerVelocity, isHit);
	ChecksIsPickUpHittingEnemy(pickUps, level);

}

bool Enemy::IsOverlapping(const Rectf& rect) const
{
	return utils::IsOverlapping(rect, m_Shape);
}

void Enemy::InitDestinationRect()
{
	//Hitbox
	m_Shape.width = /*m_Width / 2;*/	1.5f * m_Width / 4;
	//m_Shape.height = m_Height / 2;

	//Startpoint
	m_Shape.left = m_Center.x - m_Shape.width;
	m_Shape.bottom = m_Center.y;
}

void Enemy::InitSourceRect()
{
	m_SrcRect.width = m_Width;
	m_SrcRect.height = m_Height;
	m_SrcRect.bottom = 0;
	m_SrcRect.left = 0;
}

