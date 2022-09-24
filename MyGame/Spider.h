#pragma once
#include "structs.h"
#include "Vector2f.h"
#include "Enemy.h"
#include "Texture.h"
#include "SoundEffect.h"

class Spider final : public Enemy
{
public:
	explicit Spider(const Point2f& center);
	Spider(const Spider& other) = delete;
	Spider& operator=(const Spider& other) = delete;
	Spider(Spider&& other) = delete;
	Spider& operator=(Spider&& other) = delete;
	virtual ~Spider();

	void Update(const float elapsedSec, const Level& level, Rectf& player, Vector2f& playerVelocity, const Point2f& hitPoint, bool& isHit, bool& isDaed, const std::vector<PickUp*>& pickUps);
	void Draw() const;

	bool IsOverlapping(const Rectf& rect) const;
	float CalculateDistanceWithplayer(const Rectf& player);

private:
	const float m_Jump{ };
	const float m_Vision{ };
	float m_Speed{ };

	bool m_IsAbleToMove{  };
	bool m_IsAbleToMoveHorizontaly{  };
	bool m_CanMakeSound{};

	SoundEffect* m_SpiderJumpSounEffect{};
	SoundEffect* m_SpiderlandSoundEffect{};

	float DistanceFromPlayer{};

	Texture* m_pSpiderTexture{ };
	std::vector<Point2f> m_LineUnderSpider;
};

