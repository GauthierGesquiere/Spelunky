#pragma once
#include "utils.h"
#include "structs.h"
#include "Texture.h"
#include "Level.h"
#include "SoundEffect.h"

class PickUp
{

public:
	explicit PickUp(const Point2f& center);
	PickUp(const PickUp& other) = delete;
	PickUp& operator=(const PickUp& other) = delete;
	PickUp(PickUp&& other) = delete;
	PickUp& operator=(PickUp&& other) = delete;
	virtual ~PickUp();

protected:
	void Update(float elapsedSec, const Level& level, Rectf& player, Vector2f& playerVelocity,
		const Point2f& hitPoint, bool& isDestroyed, Point2f& pos, bool& isTrowing, bool& isPickedUp, bool& isOtherPickedUp);
	bool IsOverlapping(const Rectf& rect) const;

	void InitDestinationRect();
	void InitSourceRect();

	SoundEffect* m_pSoundThrowItem;
	SoundEffect* m_pSoundDropItem{};
	SoundEffect* m_pSoundPickUp;

	Point2f GetPosition() const;
	Rectf GetPickUpShape() const;

	Point2f m_Center;
	float m_Height{  };
	float m_Width{  };
	float m_Scaler{ };

	Vector2f m_Velocity{  };
	Vector2f m_Acceleration{  };
	float m_UpwardsTrow{  };

	Rectf m_SrcRect{};
	Rectf m_Shape;

	bool m_PickedUp{  };
	bool m_IsLookingRight{  };
	bool m_CanPickUp{  };
	bool m_CanMakeSound{  };

};
