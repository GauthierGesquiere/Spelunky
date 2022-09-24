#pragma once
#include <vector>
#include "structs.h"
#include "Enemies.h"

class EnemiesManager final
{
public:
	EnemiesManager( );
	~EnemiesManager( );
	EnemiesManager(const EnemiesManager&) = delete;
	EnemiesManager& operator=(const EnemiesManager&) = delete;
	EnemiesManager(EnemiesManager&&) = delete;
	EnemiesManager& operator=(EnemiesManager&&) = delete;

	Enemies* AddItem( const Point2f &center, Enemies::TypeEnemie sortEnemie);

	void Update( float elapsedSec, const Level& level, Rectf& player, Vector2f& playerVelocity, const Point2f& hitPoint, bool& isHit);
	void Draw( ) const;

	bool EnemieDamage( const Rectf& rect ) ;

private:
	std::vector<Enemies*> m_pEnemies;

};
