#pragma once
#include <vector>
#include "structs.h"
#include "Vector2f.h"
#include "Texture.h"

class Level
{
public:

	explicit Level();
	Level(const Level& other) = delete;
	Level& operator=(const Level& other) = delete;
	Level(Level&& other) = delete;
	Level& operator=(Level&& other) = delete;
	~Level();

	void DrawBackground() const;
	bool HandleCollision(Rectf& actorshape, Vector2f& actorVelocity, const bool& isPlayer) const;
	bool IsOnGround(const Rectf& actorShape, Vector2f& actorVelocity) const;
	bool IsOnledge(const Rectf& actorShape, Vector2f& actorVelocity) const;
	bool HasReachedDoor(const Rectf& actorShape);
	Rectf GetBoundaries();

private:

	void HandleCollisionFloorRoof(Rectf& actorshape, Vector2f& actorVelocity) const;
	void HandleCollisionWalls(Rectf& actorshape, Vector2f& actorVelocity) const;
	bool HandleCanGrabAledge(Rectf& actorShape, Vector2f& actorVelocity, const bool& isPlayer) const;


	mutable bool m_CanGrabledgeRight{ };
	mutable bool m_IsLookingRight{ };

	int m_Flexibility{};

	std::vector< std::vector<Point2f>> m_VectorOfVertices;
	Texture* m_pBackgroundTexture;
	Rectf m_Boundaries{};
};

