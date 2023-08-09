#include "PhaseLeave.h"
#include "Enemy.h"

void PhaseLeave::Update(Enemy* enemy)
{
	enemy->LeaveMove();
}
