#pragma once
#include <vector>
#include "structs.h"
#include "Enemy.h"
#include "Level.h"
#include "PickUp.h"

class EnemyManager final
{
public:
	explicit EnemyManager();
	~EnemyManager();
	EnemyManager(const EnemyManager&) = delete;
	EnemyManager& operator=(const EnemyManager&) = delete;
	EnemyManager(EnemyManager&&) = delete;
	EnemyManager& operator=(EnemyManager&&) = delete;

	Enemy* AddItem(const Point2f& center, std::string enemy);

	void Update(float elapsedSec, const Level& level, Rectf& player, Vector2f& playerVelocity, const Point2f& hitPoint, bool& isHit, int& enemiesKilled, const std::vector<PickUp*>& pickUps);
	void Draw() const;

	bool EnemyDamage(const Rectf& rect);

private:
	std::vector<Enemy*> m_pEnemies;

};


