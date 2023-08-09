#pragma once
#include "IEnemyState.h"

class PhaseApproach : public IEnemyState 
{
public:
	void Update(Enemy* enemy);
};