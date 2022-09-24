#include "pch.h"
#include "Enemy.h"
#include "utils.h"
#include "PickUp.h"

PickUp::PickUp(const Point2f& center)
	: m_Center{ center }
	, m_Width{ 0.0f }
	, m_Height{ 0.0f }
	, m_Velocity{ 0.0f, 0.0f }
	, m_Acceleration{ 0, -981.0f }
	, m_UpwardsTrow{ 0.0f }
	, m_PickedUp{ false }
	, m_IsLookingRight{ false }
	, m_CanPickUp{ false }
	, m_Scaler{ 0.5f }
	, m_CanMakeSound{ true }
{
	m_pSoundPickUp = new SoundEffect("Resources\\Sounds\\PickUp.wav");
	m_pSoundThrowItem = new SoundEffect("Resources\\Sounds\\Throw_Item.wav");
	m_pSoundDropItem = new SoundEffect("Resources\\Sounds\\Item_Drop.wav");
}

PickUp::~PickUp()
{
	delete m_pSoundPickUp;
	m_pSoundPickUp = nullptr;

	delete m_pSoundThrowItem;
	m_pSoundThrowItem = nullptr;

	delete m_pSoundDropItem;
	m_pSoundDropItem = nullptr;
}

void PickUp::Update(float elapsedSec, const Level& level, Rectf& player, Vector2f& playerVelocity, const Point2f& hitPoint, bool& isDestroyed, Point2f& pos, bool& isTrowing, bool& isPickedUp, bool& isOtherPickedUp)
{
	const Uint8* pKeyStates = SDL_GetKeyboardState(nullptr);

	if (!pKeyStates[SDL_SCANCODE_J])
	{
		m_CanPickUp = true;
	}

	//looks what the last direction was
	if (playerVelocity.x < 0)
	{
		m_IsLookingRight = false;
	}
	else if (playerVelocity.x > 0)
	{
		m_IsLookingRight = true;
	}

	//gravity
	m_Velocity += m_Acceleration * elapsedSec;
	m_Shape.left += m_Velocity.x * elapsedSec;
	m_Shape.bottom += m_Velocity.y * elapsedSec;

	level.HandleCollision(m_Shape, m_Velocity, false);
	if (level.IsOnGround(m_Shape, m_Velocity))
	{
		m_Velocity.x = 0;
	}

	if (!pKeyStates[SDL_SCANCODE_J])
	{
		m_CanMakeSound = true;
	}

	if ((m_Shape.left < (player.left + (player.width / 2))) && (player.left + (player.width / 2) < m_Shape.left + m_Shape.width))
	{
		//CanPickUp
		if ((m_Shape.bottom < (player.bottom + (player.height / 2))) && (player.bottom + (player.height / 2) < m_Shape.bottom + m_Shape.height))
			if (pKeyStates[SDL_SCANCODE_S] && pKeyStates[SDL_SCANCODE_J] && m_CanMakeSound && !isOtherPickedUp)
			{
				m_pSoundPickUp->SetVolume(15);
				m_pSoundPickUp->Play(0);
				m_CanPickUp = false;
				m_PickedUp = true;
				m_CanMakeSound = false;
			}
	}
	
	if (m_PickedUp)
	{
		m_Velocity.y = 0;

		m_Shape.bottom = player.bottom + 2;
		if (m_IsLookingRight)
			m_Shape.left = player.left + player.width / 2;
		else
			m_Shape.left = player.left - player.width / 2;
		if (m_CanPickUp)
		{
			if (pKeyStates[SDL_SCANCODE_J] && pKeyStates[SDL_SCANCODE_W])
			{
				m_pSoundThrowItem->SetVolume(15);
				m_pSoundThrowItem->Play(0);

				m_PickedUp = false;
				isOtherPickedUp = false;
				if (m_IsLookingRight)
					m_Velocity.x = 300;
				else
					m_Velocity.x = -300;
				m_Velocity.y = 500;

				isTrowing = true;
			}
			else if (pKeyStates[SDL_SCANCODE_J] && pKeyStates[SDL_SCANCODE_LSHIFT])
			{
				m_pSoundThrowItem->SetVolume(15);
				m_pSoundThrowItem->Play(0);

				m_PickedUp = false;
				isOtherPickedUp = false;
				if (m_IsLookingRight)
					m_Velocity.x = 600;
				else
					m_Velocity.x = -600;
				m_Velocity.y = 400;

				isTrowing = true;
			}
			else if (pKeyStates[SDL_SCANCODE_J] && pKeyStates[SDL_SCANCODE_S])
			{
				m_pSoundDropItem->SetVolume(15);
				m_pSoundDropItem->Play(0);

				m_PickedUp = false;
				isOtherPickedUp = false;
			}
			else if (pKeyStates[SDL_SCANCODE_J])
			{
				m_pSoundThrowItem->SetVolume(15);
				m_pSoundThrowItem->Play(0);

				m_PickedUp = false;
				isOtherPickedUp = false;
				if (m_IsLookingRight)
					m_Velocity.x = 500;
				else
					m_Velocity.x = -500;
				m_Velocity.y = 200;

				isTrowing = true;
			}
		}
	}
	isPickedUp = m_PickedUp;
}

bool PickUp::IsOverlapping(const Rectf& rect) const
{
	return utils::IsOverlapping(rect, m_Shape);
}

void PickUp::InitDestinationRect()
{
	//Hitbox
	m_Shape.width = m_Width / 2;
	m_Shape.height = m_Height / 2;

	//Startpoint
	m_Shape.left = m_Center.x - m_Shape.width;
	m_Shape.bottom = m_Center.y;
}

void PickUp::InitSourceRect()
{
	m_SrcRect.width = m_Width;
	m_SrcRect.height = m_Height;
	m_SrcRect.bottom = 0;
}

Point2f PickUp::GetPosition() const
{
	return Point2f{ m_Shape.left, m_Shape.bottom };
}

Rectf PickUp::GetPickUpShape() const
{
	return m_Shape;
}
