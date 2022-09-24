#pragma once
#include "PickUp.h"

class PickUpManager final 
{
public:
	explicit PickUpManager();
	~PickUpManager();
	PickUpManager(const PickUpManager&) = delete;
	PickUpManager& operator=(const PickUpManager&) = delete;
	PickUpManager(PickUpManager&&) = delete;
	PickUpManager& operator=(PickUpManager&&) = delete;

	PickUp* AddItem(const Point2f& center, std::string pickUp);

	void Update(float elapsedSec, const Level& level, Rectf& player, Vector2f& playerVelocity, const Point2f& hitPoint, bool& isDestroyed, Point2f& pos, bool& isTrowing);
	void Draw() const;

	std::vector<PickUp*>& GetPickUps();

private:
	std::vector<PickUp*> m_pPickUp;

	SoundEffect* m_pPotBreaksSoundEffect{};

	bool m_IsPickedUp{ false };
	bool m_IsOtherPickedUp{ false };

};


