#pragma once
#include "Level.h"
#include "SoundEffect.h"
#include "SoundStream.h"

class Avatar
{
public:
	explicit Avatar();
	Avatar(const Avatar& other) = delete;
	Avatar& operator=(const Avatar& other) = delete;
	Avatar(Avatar&& other) = delete;
	Avatar& operator=(Avatar&& other) = delete;
	~Avatar();

	void Update(float elapsedSec, const Level& level, bool& isHit);
	void Draw() const;
	Rectf GetShape() const;
	float GetVerticalDistance() const;
	Point2f GetHitPoint();
	Vector2f GetVelocity() const;
	void GotHit();
	int GetHealth();
	bool GetIsInDoor();
	bool GetGotHit() const;

	void SetVelocity(Vector2f velocity);
	void SetDoorAnimation(bool isInDoor, const Level& level);
	void SetTrowing(bool isTrowing);

	enum class ActionState
	{
		moving,
		waiting,
		lookingUp,
		ducking,
		duckingWhileMoving,
		jumping, 
		attacking,
		grabbingAledge,
		isDead,
		isInDoor,
		isThrowing,
		isHittingGround,
		none
	};


private:
	Point2f StartPos{ };
	Rectf m_Shape{};
	float  m_HorSpeed{  };
	float m_JumpSpeed{  };
	Vector2f m_Velocity{  };
	Vector2f m_Acceleration{  };
	ActionState m_ActionState{ };
	ActionState m_FreezeAction{  };
	float m_AccuSec{  };
	float m_MaxTransformSec{  };
	float m_AccuInvulnerableSec{ 0.0 };

	Texture* m_pTextureToDraw{};
	Texture* m_pTextureRunningCylce{};
	Texture* m_pTextureDuckingCycle{};
	Texture* m_pTextureLookingUpCycle{};
	Texture* m_pTextureJumpingCycle{};
	Texture* m_pTextureDuckingWhileMovingCycle{};
	Texture* m_pTextureWhipAttackingCycle{};
	Texture* m_pTextureCharacterAttackingCycle{};
	Texture* m_pTextureGrabbingAledgeCycle{};
	Texture* m_pTextureDeadCycle{};
	Texture* m_pTextureDoorCycle{};
	Texture* m_pTextureTrowingCycle{};
	Texture* m_pTextureHittingGroundCycle{};

	SoundEffect* m_pSoundJump;
	SoundEffect* m_pSoundLand;
	SoundEffect* m_pSoundGrab;
	SoundEffect* m_pSoundWhip;
	

	float m_ClipHeight{  };
	float m_ClipWidth{  };
	float m_AttackWidth{  };
	int m_NrOfFrames{  };
	int m_NrOfFramesAttack{  };
	int m_FramesPerSec{  };
	int m_FramesPerSecAttack{  };
	float m_CurrentTime{};
	float m_CurrentTimeAttack{};
	int m_CurrentFrame{};
	int m_CurrentFrameAttack{};
	int m_FreezeFrame{};

	float m_Scaler{  };
	float m_Speed{  };

	Rectf m_SrcRect{};
	Rectf m_SrcRectAttack{};

	int m_Rows{  };
	Point2f m_WhipOffset{  };
	Point2f m_HitPointWhip{  };

	bool m_IsGoingLeft{  }; // false = right
	bool m_IsSprinting{  };
	bool m_IsAttacking{  };
	bool m_GotHit{  };
	bool m_CanDraw{  };
	bool m_CanMoveCamera{  };
	bool m_IsInDoor{  };

	mutable float m_DirectionToWalk{  };

	bool m_GrabbedAledge{ };

	float m_ElapsedTimePressedButton{  };
	float m_ElapsedTimeHitGround{};

	bool m_CanJump{  };
	bool m_CanAttack{  };
	bool m_CanTransformCamera{  };
	bool m_IsHittingGroundToHard{};
	bool m_CanPlaySound{};

	float m_VerticalDistance{  };
	float m_MaxVerticalDistance{  };
	float m_MaxplayerVelocity{};
	float m_AccumulatedTimeWhip{};
	float m_WhipDelay{  };

	int m_HitPoints{  };

	void InitDestinationRect();
	void InitSourceRect();
	void InitSourceRectAttack();
	
	void UpdateTransforming(float elapsedSec);
	void UpdateSourceRect();
	void UpdateSourceRectAttack();
	void UpdateInputKeys(float elapsedSec, bool isOnGround);
	void UpdatingMovement(float elapsedSec);
	void UpdateTimes(float elapsedSec);
	void UpdateAttack(float elapsedSec);
	void UpdateHealth(float elapsedSec);
	void UpdateFallDamage(float elapsedSec, Vector2f playerVelocity, bool isOnGround, bool& isHit);

	bool IsPressingSomething();
	bool IsPressingNothing();

	void SetFramesPerSec(int framesPerSec);
	void SetNrOfFrames(int nrOfFrames);
	void SetFreezeFrame(int freezeFrame);
	void SetFreezeAction(ActionState actionState);
	void SetHitbox(float height);
	void SetHorizontalSpeed(float speed);
	void SetjumpSpeed(float speed);

	void DrawAttackAnimation() const;
};

