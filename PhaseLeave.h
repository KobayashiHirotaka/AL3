#pragma once
#include "IEnemyState.h"

class PhaseLeave : public IEnemyState 
{
public:
	void Update(Enemy* enemy);
};