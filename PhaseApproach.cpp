#include "PhaseApproach.h"
#include "Enemy.h"

void PhaseApproach::Update(Enemy* enemy) {

	if (enemy->GetTransform().z < 0.0f) {
		enemy->PhaseChange(new PhaseLeave());
	}
	enemy->ApproachMove();
}