#pragma once
class Camera
{
public:
	explicit Camera(float width, float height);
	void SetLevelBoundaries(const Rectf& levelBoundaries);
	void Transform(const Rectf& target, const float verticalDistance);

private:
	Point2f Track(const Rectf& target);
	void Clamp(Point2f& bottomLeftPos);

	float m_Width{};
	float m_Height{};
	Rectf m_LevelBoundaries{};
	
};

