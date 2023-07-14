#pragma once
#include "Enemy.h"
#include "Vector3.h"

class Enemy;

class IEnemyState
{
public:
	virtual ~IEnemyState(){};

	virtual void Update(Enemy* enemy) = 0;
};

class EnemyApproach : public IEnemyState 
{
public:
	void Update(Enemy* enemy);

	const Vector3 kCharacterApproachSpeed = {0.0f, 0.0f, -0.2f};
};

class EnemyLeave : public IEnemyState
{
public:
	void Update(Enemy* enemy);

	const Vector3 kCharacterLeaveSpeed = {-0.3f, 0.3f, 0.0f};
};