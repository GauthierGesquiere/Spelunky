#pragma once
#include "Enemy.h"
#include "Vector2f.h"
#include "SoundEffect.h"

class Snake final : public Enemy
{
public:
	explicit Snake(const Point2f& center);
	Snake(const Snake& other) = delete;
	Snake& operator=(const Snake& other) = delete;
	Snake(Snake&& other) = delete;
	Snake& operator=(Snake&& other) = delete;
	virtual ~Snake();

	void Update(const float elapsedSec, const Level& level, Rectf& player, Vector2f& playerVelocity, const Point2f& hitPoint, bool& isHit, bool& isDaed, const std::vector<PickUp*>& pickUps);
	void Draw() const;

	bool IsOverlapping(const Rectf& rect) const;

private:
	Texture* m_pSnakeTexture{ };

	int m_NrOfFrames{ };
	int m_FramesPerSec{ };
	float m_CurrentTime{};
	int m_CurrentFrame{};

	void UpdateTransforming(float elapsedSec);
	void UpdateSourceRect();
};

