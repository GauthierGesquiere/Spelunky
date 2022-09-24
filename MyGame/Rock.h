#pragma once
#include "PickUp.h"

class Rock final : public PickUp
{
public:
	explicit Rock(const Point2f& center);
	Rock(const Rock& other) = delete;
	Rock& operator=(const Rock& other) = delete;
	Rock(Rock&& other) = delete;
	Rock& operator=(Rock&& other) = delete;
	virtual ~Rock();

	void Update(float elapsedSec, const Level& level, Rectf& player, Vector2f& playerVelocity, const Point2f& hitPoint, bool& isDestroyed, Point2f& pos, bool& isTrowing, bool& isPickedUp, bool& isOtherPickedUp);
	void Draw() const;
	Rectf GetPickUpShape() const;
	Vector2f GetPickUpVelocity() const;

private:
	Texture* m_pRockTexture{ };
};

