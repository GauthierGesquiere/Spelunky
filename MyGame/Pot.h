#pragma once
#include "PickUp.h"
class Pot final : public PickUp
{
public:
	explicit Pot(const Point2f& center);
	Pot(const Pot& other) = delete;
	Pot& operator=(const Pot& other) = delete;
	Pot(Pot&& other) = delete;
	Pot& operator=(Pot&& other) = delete;
	virtual ~Pot();

	void Update(float elapsedSec, const Level& level, Rectf& player, Vector2f& playerVelocity, const Point2f& hitPoint, bool& isDestroyed, Point2f& pos, bool& isTrowing, bool& isPickedUp, bool& isOtherPickedUp);
	void Draw() const;
	Rectf GetPickUpShape() const;
	Vector2f GetPickUpVelocity() const;

private:
	const int m_TotalAmountOfPots{ };

	bool m_IsDestroyed{  };

	Texture* m_pPotTexture{ };
};

