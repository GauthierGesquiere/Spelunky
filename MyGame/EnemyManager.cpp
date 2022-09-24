#include "pch.h"
#include "EnemyManager.h"
#include "Snake.h"
#include "Spider.h"
#include <typeinfo>
#include <iostream>

EnemyManager::EnemyManager()
{
}

EnemyManager::~EnemyManager()
{
	for (Enemy* ptr : m_pEnemies)
	{
		/*if (typeid(*ptr) == typeid(Snake))
		{
			Snake* pSnake{ static_cast<Snake*>(ptr) };
			pSnake->~Snake();
		}
		if (typeid(*ptr) == typeid(Spider))
		{
			Spider* pSpider{ static_cast<Spider*>(ptr) };
			pSpider->~Spider();
		}*/
		delete ptr;
	}
	m_pEnemies.clear();
}

Enemy* EnemyManager::AddItem(const Point2f& center, std::string enemy)
{
	if (enemy == "Spider")
	{
		m_pEnemies.push_back(new Spider(center));
	}
	else if (enemy == "Snake")
	{
		m_pEnemies.push_back(new Snake(center));
	}
	return m_pEnemies[m_pEnemies.size() - 1];
}

void EnemyManager::Update(float elapsedSec, const Level& level, Rectf& player, Vector2f& playerVelocity, const Point2f& hitPoint, bool& isHit, int& enemiesKilled, const std::vector<PickUp*>& pickUps)
{
	int counter{ 0 };

	bool isDaed{ false };
	for (Enemy* ptr : m_pEnemies)

	{
		if (typeid(*ptr) == typeid(Snake))
		{
			Snake* pSnake{ static_cast<Snake*>(ptr) };
			pSnake->Update(elapsedSec, level, player, playerVelocity, hitPoint, isHit, isDaed, pickUps);
			if (isDaed)
			{
				enemiesKilled++;
				pSnake->~Snake();
				delete ptr;
				m_pEnemies.erase(m_pEnemies.begin() + counter);
			}
		}
		else if (typeid(*ptr) == typeid(Spider))
		{
			Spider* pSpider{ static_cast<Spider*>(ptr) };
			pSpider->Update(elapsedSec, level, player, playerVelocity, hitPoint, isHit, isDaed, pickUps);
			if (isDaed)
			{
				enemiesKilled++;
				pSpider->~Spider();
				delete ptr;
				m_pEnemies.erase(m_pEnemies.begin() + counter);
			}
		}
		counter++;
	}

}

void EnemyManager::Draw() const
{
	for (Enemy* ptr : m_pEnemies)
	{
		if (typeid(*ptr) == typeid(Snake))
		{
			Snake* pSnake{ static_cast<Snake*>(ptr) };
			pSnake->Draw();
		}
		if (typeid(*ptr) == typeid(Spider))
		{
			Spider* pSpider{ static_cast<Spider*>(ptr) };
			pSpider->Draw();
		}
	}
}


bool EnemyManager::EnemyDamage(const Rectf& rect)
{
	for (Enemy* ptr : m_pEnemies)
	{
		if (typeid(*ptr) == typeid(Snake))
		{
			Snake* pSnake{ static_cast<Snake*>(ptr) };
			if (pSnake->IsOverlapping(rect))
			{
				delete ptr;
				ptr = nullptr;
				Enemy* lastValue{ m_pEnemies[m_pEnemies.size() - 1] };
				ptr = lastValue;
				m_pEnemies.pop_back();
				return true;
			}
		}
		if (typeid(*ptr) == typeid(Spider))
		{
			Spider* pSpider{ static_cast<Spider*>(ptr) };
			if (pSpider->IsOverlapping(rect))
			{
				delete ptr;
				ptr = nullptr;
				Enemy* lastValue{ m_pEnemies[m_pEnemies.size() - 1] };
				ptr = lastValue;
				m_pEnemies.pop_back();
				return true;
			}
		}
	}
	return false;
}
