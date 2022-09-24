#include "pch.h"
#include "Avatar.h"
#include "utils.h"
#include <iostream>

Avatar::Avatar()
	: StartPos{ 1550, 1610 }
	, m_HorSpeed{ 200.0f }
	, m_JumpSpeed{ 420.0f }
	, m_Velocity{ 0.0f, 0.0f }
	, m_Acceleration{ 0, -981.0f }
	, m_ActionState{ ActionState::moving }
	, m_FreezeAction{ ActionState::none }
	, m_AccuSec{ 0.0 }
	, m_MaxTransformSec{ 2.0f }
	, m_AccuInvulnerableSec{ 0.0 }
	, m_ClipHeight{ 64.0f }
	, m_ClipWidth{ 80.0f }
	, m_AttackWidth{ 60.0f }
	, m_NrOfFrames{ 9 }
	, m_NrOfFramesAttack{ 6 }
	, m_FramesPerSec{ 10 }
	, m_FramesPerSecAttack{ 20 }
	, m_Scaler{ 0.9f }
	, m_Speed{ 0.0 }
	, m_Rows{ 1 }
	, m_WhipOffset{ -15.f, 10.f }
	, m_HitPointWhip{ 0.0f, 0.0f }
	, m_IsGoingLeft{ false }
	, m_IsSprinting{ false }
	, m_IsAttacking{ false }
	, m_GotHit{ false }
	, m_CanDraw{ true }
	, m_CanMoveCamera{ true }
	, m_DirectionToWalk{ 1.0f }
	, m_GrabbedAledge{ false }
	, m_ElapsedTimePressedButton{ 0 }
	, m_CanJump{ true }
	, m_CanAttack{ true }
	, m_CanTransformCamera{ false }
	, m_VerticalDistance{ 0 }
	, m_MaxVerticalDistance{ 300 }
	, m_HitPoints{ 3 }
	, m_MaxplayerVelocity{ -900 }
	, m_WhipDelay{0.5f}
{
	m_pTextureRunningCylce = new Texture("Resources\\Images\\RunningCycle.png");
	m_pTextureDuckingCycle = new Texture("Resources\\Images\\DuckingCycle.png");
	m_pTextureLookingUpCycle = new Texture("Resources\\Images\\LookingUpCycle.png");
	m_pTextureJumpingCycle = new Texture("Resources\\Images\\JumpingCycle.png");
	m_pTextureDuckingWhileMovingCycle = new Texture("Resources\\Images\\DuckingWhileMovingCycle.png");
	m_pTextureWhipAttackingCycle = new Texture("Resources\\Images\\WhipAttackingCycle.png");
	m_pTextureCharacterAttackingCycle = new Texture("Resources\\Images\\CharacterAttackingCycle.png");
	m_pTextureGrabbingAledgeCycle = new Texture("Resources\\Images\\GrabbingAledgeCycle.png");
	m_pTextureDeadCycle = new Texture("Resources\\Images\\DaedCycle.png");
	m_pTextureDoorCycle = new Texture("Resources\\Images\\DoorCycle.png");
	m_pTextureTrowingCycle = new Texture("Resources\\Images\\TrowingCycle.png");
	m_pTextureHittingGroundCycle = new Texture("Resources\\Images\\HittingGroundCycle.png");

	m_pSoundJump = new SoundEffect("Resources\\Sounds\\Jump.wav");
	m_pSoundLand = new SoundEffect("Resources\\Sounds\\Land.wav");
	m_pSoundGrab = new SoundEffect("Resources\\Sounds\\grab.wav");
	m_pSoundWhip = new SoundEffect("Resources\\Sounds\\WhipSound.wav");

	InitSourceRect();
	InitSourceRectAttack();
	InitDestinationRect();
}

Avatar::~Avatar()
{
	delete m_pTextureRunningCylce;
	m_pTextureRunningCylce = nullptr;

	delete m_pTextureDuckingCycle;
	m_pTextureDuckingCycle = nullptr;

	delete m_pTextureLookingUpCycle;
	m_pTextureLookingUpCycle = nullptr;

	delete m_pTextureJumpingCycle;
	m_pTextureJumpingCycle = nullptr;

	delete m_pTextureDuckingWhileMovingCycle;
	m_pTextureDuckingWhileMovingCycle = nullptr;

	delete m_pTextureWhipAttackingCycle;
	m_pTextureWhipAttackingCycle = nullptr;

	delete m_pTextureCharacterAttackingCycle;
	m_pTextureCharacterAttackingCycle = nullptr;

	delete m_pTextureGrabbingAledgeCycle;
	m_pTextureGrabbingAledgeCycle = nullptr;

	delete m_pTextureDeadCycle;
	m_pTextureDeadCycle = nullptr;

	delete m_pTextureDoorCycle;
	m_pTextureDoorCycle = nullptr;

	delete m_pTextureTrowingCycle;
	m_pTextureTrowingCycle = nullptr;

	delete m_pTextureHittingGroundCycle;
	m_pTextureHittingGroundCycle = nullptr;

	delete m_pSoundJump;
	m_pSoundJump = nullptr;

	delete m_pSoundLand;
	m_pSoundLand = nullptr;

	delete m_pSoundGrab;
	m_pSoundGrab = nullptr;

	delete m_pSoundWhip;
	m_pSoundWhip = nullptr;
}


void Avatar::Update(float elapsedSec, const Level& level, bool& isHit)
{
	const Uint8* pKeyStates = SDL_GetKeyboardState(nullptr);
	bool isOnGround = level.IsOnGround(m_Shape, m_Velocity);

	//std::cout << m_Velocity.y << std::endl;

	if (m_ActionState != ActionState::isDead)
	{
		if (m_IsAttacking)
			UpdateAttack(elapsedSec);
		UpdateInputKeys(elapsedSec, isOnGround);
		UpdateTimes(elapsedSec);
		UpdateHealth(elapsedSec);
		UpdateFallDamage(elapsedSec, m_Velocity, isOnGround, isHit);
	}
	else
		if (isOnGround)
			m_Velocity.x = 0;

	UpdateTransforming(elapsedSec);

	//std::cout << m_HitPoints << std::endl;

	switch (m_ActionState)
	{
	case Avatar::ActionState::moving:

		if (m_IsSprinting)
			SetFramesPerSec(12); // Setting frames per sec
		else
			SetFramesPerSec(8); // Setting frames per sec
		
		SetHorizontalSpeed(m_Speed);
		SetNrOfFrames(9); // Setting Nr of frames
		SetFreezeFrame(-1);
		SetFreezeAction(ActionState::none);
		SetHitbox(3 * m_ClipHeight / 4);
		SetjumpSpeed(420);

		//EveryTime when you move you have to check the movement and collision!
		//Update Postion/Physics
		UpdatingMovement(elapsedSec);

		//Detecting Collision and looking for a ledge
		if (level.HandleCollision(m_Shape, m_Velocity, true))
		{
			m_pSoundGrab->SetVolume(15);
			m_pSoundGrab->Play(0);
			m_ActionState = ActionState::grabbingAledge;
		}

		m_pTextureToDraw = m_pTextureRunningCylce; // Setting the texture needs to be drawn
		break;

	case Avatar::ActionState::waiting:	

		SetFramesPerSec(10); // Setting frames per sec
		SetNrOfFrames(1); // Setting Nr of frames
		SetFreezeFrame(-1);
		SetFreezeAction(ActionState::none);
		SetHitbox(3 * m_ClipHeight / 4);
		SetjumpSpeed(420);

		m_Velocity.x = 0.0f;

		//EveryTime when you move you have to check the movement and collision!
		//Update Postion/Physics
		UpdatingMovement(elapsedSec);

		//Detecting Collision and looking for a ledge
		if (level.HandleCollision(m_Shape, m_Velocity, true))
		{
			m_pSoundGrab->SetVolume(15);
			m_pSoundGrab->Play(0);
			m_ActionState = ActionState::grabbingAledge;
		}

		m_pTextureToDraw = m_pTextureRunningCylce; // Setting the texture needs to be drawn
		break;

	case Avatar::ActionState::ducking:

		SetFramesPerSec(7); // Setting frames per sec
		SetNrOfFrames(3); // Setting Nr of frames
		SetFreezeFrame(2);
		SetFreezeAction(m_ActionState);
		SetHitbox(m_ClipHeight / 2);
		SetjumpSpeed(420);

		m_pTextureToDraw = m_pTextureDuckingCycle;
		break;

	case Avatar::ActionState::lookingUp:

		SetFramesPerSec(10); // Setting frames per sec
		SetNrOfFrames(7); // Setting Nr of frames
		SetFreezeFrame(4);
		SetFreezeAction(m_ActionState);
		SetHitbox(3 * m_ClipHeight / 4);
		SetjumpSpeed(420);

		m_pTextureToDraw = m_pTextureLookingUpCycle;
		break;

	case Avatar::ActionState::jumping:

		if (m_GrabbedAledge)
			m_Speed = 200;

		SetHorizontalSpeed(m_Speed);
		SetFramesPerSec(8); // Setting frames per sec
		SetNrOfFrames(12); // Setting Nr of frames
		SetFreezeFrame(11);
		SetFreezeAction(m_ActionState);
		SetHitbox(3 * m_ClipHeight / 4);
		SetjumpSpeed(420);

		//EveryTime when you move you have to check the movement and collision!
		//Update Postion/Physics
		UpdatingMovement(elapsedSec);

		//Detecting Collision and looking for a ledge
		if (level.HandleCollision(m_Shape, m_Velocity, true))
		{
			m_pSoundGrab->SetVolume(15);
			m_pSoundGrab->Play(0);
			m_ActionState = ActionState::grabbingAledge;
		}

		m_pTextureToDraw = m_pTextureJumpingCycle;
		break;

	case Avatar::ActionState::duckingWhileMoving:

		SetHorizontalSpeed(100.f);
		SetFramesPerSec(10); // Setting frames per sec
		SetNrOfFrames(7); // Setting Nr of frames
		SetFreezeFrame(-1);
		SetFreezeAction(ActionState::none);
		SetHitbox(m_ClipHeight / 2);
		SetjumpSpeed(420);

		//EveryTime when you move you have to check the movement and collision!
		//Update Postion/Physics
		UpdatingMovement(elapsedSec);

		//Detecting Collision
		level.HandleCollision(m_Shape, m_Velocity, true);

		m_pTextureToDraw = m_pTextureDuckingWhileMovingCycle; // Setting the texture needs to be drawn
		break;

	case Avatar::ActionState::attacking:

		SetFramesPerSec(20); // Setting frames per sec
		SetNrOfFrames(7); // Setting Nr of frames
		SetFreezeFrame(-1);
		SetFreezeAction(ActionState::none);
		SetHitbox(3 * m_ClipHeight / 4);
		SetjumpSpeed(400);

		//EveryTime when you move you have to check the movement and collision!
		//Update Postion/Physics
		UpdatingMovement(elapsedSec);

		//Detecting Collision and looking for a ledge
		if (level.HandleCollision(m_Shape, m_Velocity, true))
		{
			m_pSoundGrab->SetVolume(15);
			m_pSoundGrab->Play(0);
			m_ActionState = ActionState::grabbingAledge;
		}

		break;

	case Avatar::ActionState::grabbingAledge:

		SetFramesPerSec(10); // Setting frames per sec
		SetNrOfFrames(1); // Setting Nr of frames
		SetFreezeFrame(-1);
		SetFreezeAction(ActionState::none);
		SetHitbox(3 * m_ClipHeight / 4);
		SetjumpSpeed(340);

		m_GrabbedAledge = true;

		m_pTextureToDraw = m_pTextureGrabbingAledgeCycle;
		break;

	case Avatar::ActionState::isDead:

		SetFramesPerSec(10); // Setting frames per sec
		SetNrOfFrames(4); // Setting Nr of frames
		SetFreezeFrame(3);
		SetFreezeAction(ActionState::isDead);
		SetHitbox(m_ClipHeight / 2);
		SetjumpSpeed(340);

		//EveryTime when you move you have to check the movement and collision!
		//Update Postion/Physics
		UpdatingMovement(elapsedSec);

		//Detecting Collision and looking for a ledge
		level.HandleCollision(m_Shape, m_Velocity, true);

		m_pTextureToDraw = m_pTextureDeadCycle;
		break;

	case Avatar::ActionState::isInDoor:

		SetFramesPerSec(10); // Setting frames per sec
		SetNrOfFrames(4); // Setting Nr of frames
		SetFreezeFrame(3);
		SetFreezeAction(ActionState::isDead);
		SetHitbox(m_ClipHeight / 2);
		SetjumpSpeed(340);

		m_pTextureToDraw = m_pTextureDoorCycle;
		break;

	case Avatar::ActionState::isThrowing:

		SetFramesPerSec(10); // Setting frames per sec
		SetNrOfFrames(4); // Setting Nr of frames
		SetFreezeFrame(-1);
		SetFreezeAction(ActionState::none);
		SetHitbox(m_ClipHeight / 2);
		SetjumpSpeed(340);

		//EveryTime when you move you have to check the movement and collision!
		//Update Postion/Physics
		UpdatingMovement(elapsedSec);

		//Detecting Collision and looking for a ledge
		level.HandleCollision(m_Shape, m_Velocity, true);


		m_pTextureToDraw = m_pTextureTrowingCycle;
		break;

	case Avatar::ActionState::isHittingGround:

		SetFramesPerSec(10); // Setting frames per sec
		SetNrOfFrames(4); // Setting Nr of frames
		SetFreezeFrame(3);
		SetFreezeAction(ActionState::isHittingGround);
		SetHitbox(m_ClipHeight / 2);
		SetjumpSpeed(340);

		//EveryTime when you move you have to check the movement and collision!
		//Update Postion/Physics
		UpdatingMovement(elapsedSec);

		//Detecting Collision and looking for a ledge
		level.HandleCollision(m_Shape, m_Velocity, true);


		m_pTextureToDraw = m_pTextureHittingGroundCycle;
		break;

	default:
		break;
	}
}

void Avatar::Draw() const
{
	//Debug code
	//utils::SetColor(Color4f{ 1,1,1,1 });
	//utils::DrawRect(m_Shape);	//Hitbox
	//utils::DrawLine(Point2f{ m_Shape.left + (m_Shape.width / 2), (m_Shape.bottom + m_Shape.height) }	//MiddleOfThe character
	//, Point2f{ m_Shape.left + (m_Shape.width / 2), m_Shape.bottom });
	//utils::DrawLine(Point2f{ m_Shape.left - (m_Shape.width / 4), (m_Shape.bottom + m_Shape.height) }
	//, Point2f{ m_Shape.left + (m_Shape.width / 4), (m_Shape.bottom + m_Shape.height) });	//Line to grab a ledge
	//utils::DrawLine(Point2f{ m_Shape.left + m_Shape.width + (m_Shape.width / 4), (m_Shape.bottom + m_Shape.height) }
	//, Point2f{ m_Shape.left + (m_Shape.width / 4), (m_Shape.bottom + m_Shape.height) });	//Line to grab a ledge

	/*utils::SetColor(Color4f{ 1,0,0,1 });
	utils::DrawLine(Point2f{ 0, 0 }, m_HitPointWhip, 3)*/;

	if (!m_IsAttacking)
	{
		m_DirectionToWalk = 1.0f;
		if (m_IsGoingLeft)
			m_DirectionToWalk = -1.0f;
	}

	glPushMatrix();
		glTranslatef(m_Shape.left, m_Shape.bottom, 0);
		glTranslatef(m_Shape.width / 2.0f, 0.0f, 0.0f);// half of the with pivot BACK translation
		glScalef(m_DirectionToWalk * m_Scaler, m_Scaler, 1.0f);
		glTranslatef(-m_Shape.width * 1.35f, 0.0f, 0.0f); // half of the with pivot translation
		if (m_IsAttacking)
			DrawAttackAnimation();
		else
		{
			if (m_GotHit)
			{
				if (m_CanDraw)
					m_pTextureToDraw->Draw(Point2f(0.0f, 0.0f), m_SrcRect);	
			}
			else
			{
				m_pTextureToDraw->Draw(Point2f(0.0f, 0.0f), m_SrcRect);
			}
		}
	glPopMatrix();
}

Rectf Avatar::GetShape() const
{
	return m_Shape;
}

float Avatar::GetVerticalDistance() const
{
	return m_VerticalDistance;
}

Point2f Avatar::GetHitPoint()
{
	return m_HitPointWhip;
}

Vector2f Avatar::GetVelocity() const
{
	return m_Velocity;
}

void Avatar::GotHit()
{
	m_HitPoints--;
	m_GotHit = true;
}

int Avatar::GetHealth()
{
	return m_HitPoints;
}

bool Avatar::GetIsInDoor()
{
	return m_IsInDoor;
}

bool Avatar::GetGotHit() const
{
	return m_GotHit;
}

void Avatar::UpdateTransforming(float elapsedSec)
{
	m_CurrentTime += elapsedSec;
	if (m_CurrentTime > (1.f / m_FramesPerSec))
	{
		if (m_ActionState == m_FreezeAction && m_CurrentFrame == m_FreezeFrame)
		{
			m_CurrentFrame = m_FreezeFrame;
		}
		else
			++m_CurrentFrame %= (m_NrOfFrames * m_Rows);
		if (m_ActionState == ActionState::isThrowing && m_CurrentFrame == m_NrOfFrames - 1)
		{
			m_ActionState = ActionState::waiting;
		}
		m_CurrentTime -= (1.f / m_FramesPerSec);
		UpdateSourceRect();
	}

}

void Avatar::InitDestinationRect()
{
	//HitBox
	m_Shape.width =  1.5f * m_ClipWidth / 4;
	m_Shape.height = 3 * m_ClipHeight / 4;

	//Startpoint DOOR
	m_Shape.left = StartPos.x;
	m_Shape.bottom = StartPos.y;
}

void Avatar::InitSourceRect()
{
	m_SrcRect.width = m_ClipWidth;
	m_SrcRect.height = m_ClipHeight;
	m_SrcRect.bottom = 0;
	UpdateSourceRect();
}

void Avatar::InitSourceRectAttack()
{
	m_SrcRectAttack.width = m_AttackWidth;
	m_SrcRectAttack.height = m_ClipHeight;
	m_SrcRectAttack.bottom = 0;
	UpdateSourceRectAttack();
}

void Avatar::UpdateSourceRect()
{
	m_SrcRect.left = m_CurrentFrame % m_NrOfFrames * m_SrcRect.width;
}

void Avatar::UpdateSourceRectAttack()
{
	m_SrcRectAttack.left = m_CurrentFrameAttack % m_NrOfFramesAttack * m_SrcRectAttack.width;
}

void Avatar::UpdateInputKeys(float elapsedSec, bool isOnGround)
{
	const Uint8* pKeyStates = SDL_GetKeyboardState(nullptr);

	m_AccumulatedTimeWhip += elapsedSec;

	if (m_ActionState == ActionState::isThrowing || m_ActionState == ActionState::isHittingGround)
		return;

	if (!isOnGround)
	{
		m_CanPlaySound = true;
	}

	if (isOnGround && m_CanPlaySound)
	{
		m_CanPlaySound = false;
		m_pSoundLand->SetVolume(15);
		m_pSoundLand->Play(0);
	}

	if (m_ActionState == ActionState::jumping)
	{
		if (isOnGround)
		{
			if (!m_IsAttacking)
			{
				m_ActionState = ActionState::waiting;
			}
		}
	}

	if (!pKeyStates[SDL_SCANCODE_SPACE])
	{
		m_CanJump = true;
	}

	if (!pKeyStates[SDL_SCANCODE_K]) //Attacking
	{
		if (m_AccumulatedTimeWhip >= m_WhipDelay)
		{
			m_AccumulatedTimeWhip = 0.0f;
			m_CanAttack = true;
		}
	}

	if (pKeyStates[SDL_SCANCODE_LSHIFT])
	{
		m_IsSprinting = true;
		m_Speed = 400;
	}
	else
	{
		m_IsSprinting = false;
		m_Speed = 200;
	}

	if (!pKeyStates[SDL_SCANCODE_D])
		m_Velocity.x = 0;

	if (!pKeyStates[SDL_SCANCODE_A])
		m_Velocity.x = 0;

	if (m_ActionState != ActionState::grabbingAledge)
	{
		if (pKeyStates[SDL_SCANCODE_D])
		{
			m_CanTransformCamera = false;

			m_Velocity.x = m_HorSpeed;
			m_IsGoingLeft = false;
			if (m_ActionState != ActionState::jumping)
			{
				m_ActionState = ActionState::moving;
			}
		}

		if (pKeyStates[SDL_SCANCODE_A])
		{
			m_CanTransformCamera = false;

			m_Velocity.x = -m_HorSpeed;
			m_IsGoingLeft = true;
			if (m_ActionState != ActionState::jumping)
			{
				m_ActionState = ActionState::moving;
			}
		}
	}

	if (m_ActionState == ActionState::grabbingAledge)
	{
		if (pKeyStates[SDL_SCANCODE_SPACE])
		{
			m_CanTransformCamera = false;

			m_ActionState = ActionState::jumping;
			m_Velocity.y = m_JumpSpeed;
			m_CanJump = false;
		}

		if (pKeyStates[SDL_SCANCODE_S] && pKeyStates[SDL_SCANCODE_SPACE])
		{
			m_Velocity.y = -200;
			m_ActionState = ActionState::waiting;
		}
	}

	if (isOnGround)
	{
		m_GrabbedAledge = false;

		if (IsPressingNothing())
		{
			m_CanTransformCamera = false;
			if (!m_IsAttacking)
			{
				if (m_ActionState != ActionState::isInDoor)
				{
					m_ActionState = ActionState::waiting;
				}
			}
		}

		if (m_CanJump) //So you cant jump repeatedly
		{
			if (pKeyStates[SDL_SCANCODE_SPACE])
			{
				m_pSoundJump->SetVolume(15);
				m_pSoundJump->Play(0);

				m_CanTransformCamera = false;

				m_ActionState = ActionState::jumping;
				m_Velocity.y = m_JumpSpeed;
				m_CanJump = false;
			}
		}

		if (pKeyStates[SDL_SCANCODE_S])
		{
			m_CanTransformCamera = true;
			m_ActionState = ActionState::ducking;

			if (pKeyStates[SDL_SCANCODE_D])
			{
				m_CanTransformCamera = false;

				m_Velocity.x = m_HorSpeed;
				m_IsGoingLeft = false;
				m_ActionState = ActionState::duckingWhileMoving;
			}

			if (pKeyStates[SDL_SCANCODE_A])
			{
				m_CanTransformCamera = false;

				m_Velocity.x = -m_HorSpeed;
				m_IsGoingLeft = true;
				m_ActionState = ActionState::duckingWhileMoving;
			}
		}

		if (pKeyStates[SDL_SCANCODE_W])
		{
			m_CanTransformCamera = true;
			m_ActionState = ActionState::lookingUp;
		}
	}

	if (m_CanAttack)
	{
		if (m_ActionState != ActionState::grabbingAledge)
		{
			if (pKeyStates[SDL_SCANCODE_K]) //Attacking
			{
				m_pSoundWhip->SetVolume(15);
				m_pSoundWhip->Play(0);
				m_ActionState = ActionState::attacking;
				m_IsAttacking = true;
				m_CanAttack = false;
			}
		}
	}
}

bool Avatar::IsPressingSomething()
{
	const Uint8* pKeyStates = SDL_GetKeyboardState(nullptr);

	return pKeyStates[SDL_SCANCODE_A] || pKeyStates[SDL_SCANCODE_D] || pKeyStates[SDL_SCANCODE_SPACE];
}

bool Avatar::IsPressingNothing()
{
	const Uint8* pKeyStates = SDL_GetKeyboardState(nullptr);

	return !pKeyStates[SDL_SCANCODE_A] && !pKeyStates[SDL_SCANCODE_D] && !pKeyStates[SDL_SCANCODE_SPACE] && 
		!pKeyStates[SDL_SCANCODE_S] && !pKeyStates[SDL_SCANCODE_W];
}

void Avatar::DrawAttackAnimation() const
{
	m_pTextureWhipAttackingCycle->Draw(m_WhipOffset, m_SrcRectAttack);
	m_pTextureCharacterAttackingCycle->Draw(Point2f{ 0.0f, 0.0f }, m_SrcRect);
}

void Avatar::SetFramesPerSec(int framesPerSec)
{
	m_FramesPerSec = framesPerSec;
}

void Avatar::SetNrOfFrames(int nrOfFrames)
{
	m_NrOfFrames = nrOfFrames;
}

void Avatar::UpdatingMovement(float elapsedSec)
{
	m_Velocity += m_Acceleration * elapsedSec;
	m_Shape.left += m_Velocity.x * elapsedSec;
	m_Shape.bottom += m_Velocity.y * elapsedSec;
}

void Avatar::UpdateTimes(float elapsedSec)
{
	const Uint8* pKeyStates = SDL_GetKeyboardState(nullptr);

	if (m_CanTransformCamera)
	{
		m_ElapsedTimePressedButton += elapsedSec;
	}

	if (m_ElapsedTimePressedButton >= 1)
	{
		m_CanTransformCamera = false;
		if (m_CanMoveCamera) 
		{
			if (m_ActionState == ActionState::lookingUp)
			{
				if (m_VerticalDistance < m_MaxVerticalDistance)
				{
					m_VerticalDistance += 6;
				}
			}
			else if (m_ActionState == ActionState::ducking)
			{
				if (m_VerticalDistance > -m_MaxVerticalDistance)
				{
					m_VerticalDistance -= 6;
				}
			}
		}
		if ((m_ActionState != ActionState::ducking && m_ActionState != ActionState::lookingUp) || m_CanMoveCamera == false)
		{
			m_CanMoveCamera = false;

			if (m_VerticalDistance > 0)
			{
				if (((m_Shape.bottom + (m_Shape.height / 2)) + m_VerticalDistance) > (m_Shape.bottom + (m_Shape.height / 2)))
				{
					m_VerticalDistance -=6 ;
				}
				else 
				{					
					m_ElapsedTimePressedButton = 0;
					m_CanMoveCamera = true;
				}
			}
			else
			{
				if (((m_Shape.bottom + (m_Shape.height / 2)) + m_VerticalDistance) < (m_Shape.bottom + (m_Shape.height / 2)))
				{
					m_VerticalDistance += 6;
				}
				else
				{
					m_ElapsedTimePressedButton = 0;
					m_CanMoveCamera = true;
				}
			}
		}
	}
	else
		if (pKeyStates[SDL_SCANCODE_J])
		{
			m_ElapsedTimePressedButton = 0;
		}
}

void Avatar::UpdateAttack(float elapsedSec)
{
	Point2f offset{ m_Shape.left + (m_Shape.width / 2.0f) - (m_Shape.width * 1.35f), m_Shape.bottom };
	switch (m_CurrentFrameAttack)
	{
	case 0:
		m_WhipOffset.x = -15;
		m_WhipOffset.y = 10;
		m_HitPointWhip.x = offset.x + 5;
		m_HitPointWhip.y = offset.y + 20;
		if (m_DirectionToWalk < 0)
			m_HitPointWhip.x = offset.x + 75;

		break;
	case 1:
		m_WhipOffset.x = -15;
		m_WhipOffset.y = 10;
		m_HitPointWhip.x = offset.x;
		m_HitPointWhip.y = offset.y + 35;
		if (m_DirectionToWalk < 0) 
		{
			m_HitPointWhip.x = offset.x + 80;
			m_HitPointWhip.y = offset.y + 30;
		}
		break;
	case 2:
		m_WhipOffset.x = -15;
		m_WhipOffset.y = 10;
		m_HitPointWhip.x = offset.x;
		m_HitPointWhip.y = offset.y + 35;
		if (m_DirectionToWalk < 0)
			m_HitPointWhip.x = offset.x + 75;
		
		break;
	case 3:
		m_WhipOffset.x = 0;
		m_WhipOffset.y = 20;
		m_HitPointWhip.x = offset.x + 5;
		m_HitPointWhip.y = offset.y + 50;
		if (m_DirectionToWalk < 0)
			m_HitPointWhip.x = offset.x + 70;

		break;
	case 4:
		m_WhipOffset.x = 45;
		m_WhipOffset.y = -8;
		m_HitPointWhip.x = offset.x + 95;
		m_HitPointWhip.y = offset.y + 30;
		if (m_DirectionToWalk < 0)
			m_HitPointWhip.x = offset.x - 5;
	
		break;
	case 5:
		m_WhipOffset.x = 45;
		m_WhipOffset.y = -10;
		m_HitPointWhip.x = offset.x + 100;
		m_HitPointWhip.y = offset.y + 20;
		if (m_DirectionToWalk < 0)
			m_HitPointWhip.x = offset.x - 5;
		
		break;
	default:
		break;
	}

	m_CurrentTimeAttack += elapsedSec;
	if (m_CurrentTimeAttack > (1.f / m_FramesPerSecAttack))
	{
		if (m_NrOfFramesAttack - 1 <= m_CurrentFrameAttack)
		{
			m_IsAttacking = false;
			m_HitPointWhip.x = 0;
			m_HitPointWhip.y = 0;
		}

		++m_CurrentFrameAttack %= m_NrOfFramesAttack;
		m_CurrentTimeAttack -= (1.f / m_FramesPerSecAttack);
		UpdateSourceRectAttack();
	}
}

void Avatar::UpdateHealth(float elapsedSec)
{
	if (m_GotHit)
	{
		m_AccuSec += elapsedSec;
		if (m_AccuSec > m_MaxTransformSec)
		{
			m_AccuSec = 0;
			m_GotHit = false;
		}

		m_AccuInvulnerableSec += elapsedSec;
		if (m_AccuInvulnerableSec > m_MaxTransformSec / 24.f)
		{
			m_CanDraw = !m_CanDraw; //make the avatar vissible or not(effect)
			m_AccuInvulnerableSec = 0;
		}
	}
	else
		m_CanDraw = true;

	if (m_HitPoints < 1)
	{
		m_CanDraw = true;
		m_ActionState = ActionState::isDead;
	}
}

void Avatar::UpdateFallDamage(float elapsedSec, Vector2f playerVelocity, bool isOnGround, bool& isHit)
{
	if (playerVelocity.y < m_MaxplayerVelocity)
	{
		m_IsHittingGroundToHard = true;
	}

	if (m_IsHittingGroundToHard)
	{
		if (isOnGround)
		{
			m_ActionState = ActionState::isHittingGround;
			isHit = true;
			m_IsHittingGroundToHard = false;
		}
	}

	if (m_ActionState == ActionState::isHittingGround)
	{
		if (isOnGround)
		{
			m_ElapsedTimeHitGround += elapsedSec;
		}
		
		if (m_ElapsedTimeHitGround > 2)
		{
			m_ActionState = ActionState::moving;
			m_ElapsedTimeHitGround = 0.0f;
		}
	}
}

void Avatar::SetFreezeFrame(int freezeFrame)
{
	m_FreezeFrame = freezeFrame;
}

void Avatar::SetFreezeAction(ActionState actionState)
{
	m_FreezeAction = actionState;
}

void Avatar::SetHitbox(float height)
{
	m_Shape.height = height;
}

void Avatar::SetHorizontalSpeed(float speed)
{
	m_HorSpeed = speed;
}

void Avatar::SetjumpSpeed(float speed)
{
	m_JumpSpeed = speed;
}

void Avatar::SetVelocity(Vector2f velocity)
{
	m_Velocity = velocity;
}

void Avatar::SetDoorAnimation(bool canGoInDoor, const Level& level)
{
	const Uint8* pKeyStates = SDL_GetKeyboardState(nullptr);
	bool isOnGround = level.IsOnGround(m_Shape, m_Velocity);

	if (canGoInDoor && pKeyStates[SDL_SCANCODE_H] && isOnGround)
	{
		m_IsInDoor = true;
		m_ActionState = ActionState::isInDoor;
	}
}

void Avatar::SetTrowing(bool isTrowing)
{
	if (isTrowing)
	{
		m_ActionState = ActionState::isThrowing;
	}
}


