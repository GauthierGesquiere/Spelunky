#include "pch.h"
#include "PickUpManager.h"
#include "Rock.h"
#include "Pot.h"
#include <typeinfo>
#include <iostream>


PickUpManager::PickUpManager()
	: m_IsPickedUp {false}
	, m_IsOtherPickedUp{ false }
{
	m_pPotBreaksSoundEffect = new SoundEffect("Resources\\Sounds\\Clay Pot Break Sound Effect - HD Quality.wav");
}

PickUpManager::~PickUpManager()
{
	for (PickUp* ptr : m_pPickUp)
	{
		if (typeid(*ptr) == typeid(Rock))
		{
			Rock* pRock{ static_cast<Rock*>(ptr) };
			pRock->~Rock();
		}
		if (typeid(*ptr) == typeid(Pot))
		{
			Pot* pPot{ static_cast<Pot*>(ptr) };
			pPot->~Pot();
		}
		delete ptr;
	}
	m_pPickUp.clear();

	delete m_pPotBreaksSoundEffect;
	m_pPotBreaksSoundEffect = nullptr;
}

PickUp* PickUpManager::AddItem(const Point2f& center, std::string pickUp)
{
	if (pickUp == "Rock")
	{
		m_pPickUp.push_back(new Rock(center));
	}
	else if (pickUp == "Pot")
	{
		m_pPickUp.push_back(new Pot(center));
	}
	return m_pPickUp[m_pPickUp.size() - 1];
}

void PickUpManager::Update(float elapsedSec, const Level& level, Rectf& player, Vector2f& playerVelocity, const Point2f& hitPoint, bool& isDestroyed, Point2f& pos, bool& isTrowing)
{
	int counter{ 0 };

	for (PickUp* ptr : m_pPickUp)
	{
		if (typeid(*ptr) == typeid(Rock))
		{
			Rock* pRock{ static_cast<Rock*>(ptr) };
			pRock->Update(elapsedSec, level, player, playerVelocity, hitPoint, isDestroyed, pos, isTrowing, m_IsPickedUp, m_IsOtherPickedUp);
			if (m_IsPickedUp)
			{
				m_IsOtherPickedUp = true;
			}
		}
		else if (typeid(*ptr) == typeid(Pot))
		{
			Pot* pPot{ static_cast<Pot*>(ptr) };
			pPot->Update(elapsedSec, level, player, playerVelocity, hitPoint, isDestroyed, pos, isTrowing, m_IsPickedUp, m_IsOtherPickedUp);
			if (m_IsPickedUp)
			{
				m_IsOtherPickedUp = true;
			}
			if (isDestroyed)
			{
				m_pPotBreaksSoundEffect->SetVolume(15);
				m_pPotBreaksSoundEffect->Play(0);
				pPot->~Pot();
				delete ptr;
				m_pPickUp.erase(m_pPickUp.begin() + counter);
			}
		}
		counter++;
	}
}

void PickUpManager::Draw() const
{
	for (PickUp* ptr : m_pPickUp)
	{
		if (typeid(*ptr) == typeid(Rock))
		{
			Rock* pRock{ static_cast<Rock*>(ptr) };
			pRock->Draw();
		}
		else if (typeid(*ptr) == typeid(Pot))
		{
			Pot* pPot{ static_cast<Pot*>(ptr) };
			pPot->Draw();
		}
	}
}

std::vector<PickUp*>& PickUpManager::GetPickUps()
{
	return m_pPickUp;
}


