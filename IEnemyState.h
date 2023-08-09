#pragma once
#pragma once
#include "Affine.h"
#include "WorldTransformEx.h"
#include <WorldTransform.h>

class Enemy;

class IEnemyState
{
public:
	virtual ~IEnemyState();

	virtual void Update(Enemy* enemy) = 0;
};

inline IEnemyState::~IEnemyState() 
{

}