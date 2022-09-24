#pragma once
#include "Level.h"
#include "Texture.h"

class Enemies final
{
public:
	enum class TypeEnemie
	{
		Spider,
		Snake,
	};

	explicit Enemies(const Point2f& center, Enemies::TypeEnemie sortEnemie);
	~Enemies();
	void Update(float elapsedSec, const Level& level, Rectf& player, Vector2f& playerVelocity, const Point2f& hitPoint, bool& isHit, bool& isDaed);
	void Draw() const;
	bool IsOverlapping(const Rectf& rect) const;

private:
	void InitDestinationRect();
	void InitSourceRect();

	void UpdateTransforming(float elapsedSec);
	void UpdateSourceRect();

	Rectf m_SrcRect{};
	float m_ClipHeight{ };
	float m_ClipWidth{ };
	int m_NrOfFrames{ };
	int m_FramesPerSec{ };
	float m_CurrentTime{};
	int m_CurrentFrame{};

	float m_Direction{  }; // 1 = right
	float m_ElapsedSecOnGround{  };
	float m_InvulnerabilitySec{  };

	const TypeEnemie m_Enemie;
	const Texture* m_pTextureToDraw;

	Vector2f m_Velocity{  };
	float m_Spiderjump{  };
	float m_SpiderSpeed{  };
	Vector2f m_Acceleration{ };
	float m_Scaler{  };
	float m_Bounce{  };
	float m_SpiderVision{};

	Rectf m_Shape;
	Point2f m_StartPos;

	std::vector<Point2f> m_Vertices;

	bool m_IsAbleToMove{  };
	bool m_IsAbleToMoveHorizontaly{  };
	bool m_IsKilled{  };
};


