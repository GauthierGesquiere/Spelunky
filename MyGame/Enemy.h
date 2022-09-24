#pragma once
#include "structs.h"
#include "Vector2f.h"
#include "Level.h"
#include "PickUp.h"

class Enemy
{
public:
	explicit Enemy(const Point2f& center);
	Enemy(const Enemy& other) = delete;
	Enemy& operator=(const Enemy& other) = delete;
	Enemy(Enemy&& other) = delete;
	Enemy& operator=(Enemy&& other) = delete;
	virtual ~Enemy();

private:
	void ChecksIsOnTopOfEnemy(Rectf& player, Vector2f& playerVelocity);
	void ChecksIsPickUpHittingEnemy(const std::vector<PickUp*>& pickUps, const Level& level);
	void ChecksIsInEnemy(Rectf& player, Vector2f& playerVelocity, bool& isHit);

protected:
	void Update(const float elapsedSec, const Level& level, Rectf& player, Vector2f& playerVelocity, const Point2f& hitPoint, bool& isHit, const std::vector<PickUp*>& pickUps);
	bool IsOverlapping(const Rectf& rect) const;

	void InitDestinationRect();
	void InitSourceRect();

	Point2f m_Center{ };
	float m_Width{  };
	float m_Height{  };

	Rectf m_SrcRect{};

	float m_Direction{  }; // 1 = right
	float m_ElapsedSecOnGround{  };
	float m_InvulnerabilitySec{  };

	Vector2f m_Velocity{  };
	Vector2f m_Acceleration{ };
	float m_Scaler{  };
	float m_Bounce{  };

	Rectf m_Shape;

	bool m_IsKilled{  };

	int m_EnemiesDied{};
};

