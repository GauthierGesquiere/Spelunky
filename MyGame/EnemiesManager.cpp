#include "pch.h"
#include "EnemiesManager.h"

EnemiesManager::EnemiesManager()
{

}

EnemiesManager::~EnemiesManager()
{
	for (Enemies* pEnemie : m_pEnemies)
	{
		delete pEnemie;
	}
	m_pEnemies.clear();
}

Enemies* EnemiesManager::AddItem(const Point2f& center, Enemies::TypeEnemie sortEnemie)
{
	m_pEnemies.push_back(new Enemies(center, sortEnemie));
	return m_pEnemies[m_pEnemies.size() - 1];
}

void EnemiesManager::Update(float elapsedSec, const Level& level, Rectf& player, Vector2f& playerVelocity, const Point2f& hitPoint, bool& isHit)
{
	bool isDaed{ false };
	for (size_t i = 0; i < m_pEnemies.size(); i++)
	{
		m_pEnemies[i]->Update(elapsedSec, level, player, playerVelocity, hitPoint, isHit, isDaed);

		if (isDaed)
		{
			m_pEnemies[i]->~Enemies();
			delete m_pEnemies[i];

			m_pEnemies.erase(m_pEnemies.begin() + i);
		}
	}
}

void EnemiesManager::Draw() const
{
	for (size_t i = 0; i < m_pEnemies.size(); i++)
	{
		m_pEnemies[i]->Draw();
	}
}


bool EnemiesManager::EnemieDamage(const Rectf& rect)
{
	for (size_t i = 0; i < m_pEnemies.size(); i++)
	{
		if (m_pEnemies[i]->IsOverlapping(rect))
		{
			delete m_pEnemies[i];
			m_pEnemies[i] = nullptr;
			Enemies* lastValue{ m_pEnemies[m_pEnemies.size() - 1] };
			m_pEnemies[i] = lastValue;
			m_pEnemies.pop_back();
			return true;
		}
	}
	return false;
}
