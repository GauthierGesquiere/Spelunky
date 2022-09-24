#pragma once
#include "Vector2f.h"
#include "texture.h"
#include <vector>

class Platform
{
public: 
	Platform(const Point2f& bottomLeft);
	~Platform();
	void Draw() const;
	void HandleCollision(Rectf& actorShape, Vector2f& actorVelocity);
	bool IsOnGround(const Rectf& actorShape, const Vector2f& actorVelocity);
	
private:
	Rectf m_Shape{ 0, 0, 0, 0 };
	Texture* m_pTexture{0};
	std::vector<Point2f> m_Vector;
};

